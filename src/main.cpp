#include <Arduino.h>

// Define os pinos usados
#define VCC_bot1 19
#define SIG_bot1 21
#define VCC_bot2 5
#define SIG_bot2 18
#define VCC_pot 25
#define GND_pot 34
#define SIG_pot 32
#define SCK_cell 12 // Amarelo
#define DT_cell 13 // Verde
#define RPWM_ph 27
#define LPWM_ph 14

// Define as constantes
const int offset1_ADC = 0; // Offset do ADC com o braço abaixado
const int offset2_ADC = 4095; // Offset do ADC com o braço levantado
const int pos1_pot = 0; // Posição angular do potenciômetro com o braço abaixado
const int pos2_pot = 120; // Posição angular do potenciômetro com o braço levantado (trocar para 90°?)
const int freq_PWM = 20000; // Frequência do PWM
const int res_PWM = 10; // Resolução em bits do PWM
const int canal_RPWM = 0; // Canal responsável pelo RPWM
const int canal_LPWM = 1; // Canal responsável pelo LPWM
const int Int_sat = 512; // Anti-windup
const float Kp = 0; // (Tunar depois)
const float Ki = 0; // (Tunar depois)
const float Kd = 0; // (Tunar depois)
const float dt = 0.01; // Período de amostragem do PID
const float fila_init = 0; // Valor inicial das filas

// Inicializa as handles
QueueHandle_t fila_pos = NULL;
QueueHandle_t fila_ref = NULL;

// Declara as tarefas
void Task_DeadMan(void *pv);
void Task_LerSens(void *pv);
void Task_ExecPID(void *pv);
void Task_RotFisi(void *pv);
void Task_LerCell(void *pv);
void Task_Comunic(void *pv);

// Declara as funções
float map_float(float var,float de_min,float de_max,float para_min,float para_max);
float ler_pot();

// Setup do código
void setup() {
  Serial.begin(115200);
  
  // Cria novas portas de sinal
  pinMode(VCC_bot1,OUTPUT);
  pinMode(VCC_bot2,OUTPUT);
  pinMode(VCC_pot,OUTPUT);
  pinMode(GND_pot,OUTPUT);
  digitalWrite(VCC_bot1,HIGH);
  digitalWrite(VCC_bot2,HIGH);
  digitalWrite(VCC_pot,HIGH);
  digitalWrite(GND_pot,LOW);
  
  // Configura as saídas PWM dos pinos da ponte h
  ledcSetup(canal_RPWM,freq_PWM,res_PWM); // Configura a frequência e a resolução do canal do RPWM
  ledcSetup(canal_LPWM,freq_PWM,res_PWM); // Configura a frequência e a resolução do canal do LPWM
  ledcAttachPin(RPWM_ph,canal_RPWM);
  ledcAttachPin(LPWM_ph,canal_LPWM);

  // Cria as filas
  fila_pos = xQueueCreate(1,sizeof(float));
  fila_ref = xQueueCreate(1,sizeof(float));

  // Inicializa as filas
  xQueueOverwrite(fila_pos,&fila_init);
  xQueueOverwrite(fila_ref,&fila_init);
  
  // Cria as tarefas
  xTaskCreatePinnedToCore(Task_DeadMan,"DeadMan",2048,NULL,5,NULL,1);
  xTaskCreatePinnedToCore(Task_LerSens,"LerSens",2048,NULL,4,NULL,1);
  xTaskCreatePinnedToCore(Task_ExecPID,"ExecPID",2048,NULL,3,NULL,1);
  xTaskCreatePinnedToCore(Task_RotFisi,"RotFisi",2048,NULL,2,NULL,1);
  xTaskCreatePinnedToCore(Task_LerCell,"LerCell",2048,NULL,2,NULL,1);
  xTaskCreatePinnedToCore(Task_Comunic,"Comunic",2048,NULL,1,NULL,0);
}

// Loop no FreeRTOS deve ficar vazio
void loop() {
  vTaskDelete(NULL);
}

// Define as tarefas
void Task_DeadMan(void *pvParameters) {}

void Task_LerSens(void *pvParameters) {
  // Guarda o tick em que a tarefa começou
  TickType_t wake = xTaskGetTickCount();
  
  for(;;) {
    //Lê o potenciômetro
    float pos_pot = ler_pot();

    // Guarda a posição do potenciômetro na fila
    xQueueOverwrite(fila_pos,&pos_pot);
    
    // Período da tarefa
    xTaskDelayUntil(&wake,pdMS_TO_TICKS(5));
  }
}

void Task_ExecPID(void *pvParameters) {
  // Inicializa as variáveis locais
  float pos_pot = 0;
  float ref = 0;
  float erro = 0;
  float erro_ant = 0;
  float P = 0;
  float I = 0;
  float D = 0;
  float PID = 0;
  float Int = 0;
  float Der = 0;
  
  // Guarda o tick em que a tarefa começou
  TickType_t wake = xTaskGetTickCount();
  
  // Inicializa o erro anterior para evitar tranco de partida devido ao termo derivativo
  xQueuePeek(fila_pos,&pos_pot,0);
  xQueuePeek(fila_ref,&ref,0);
  erro_ant = ref - pos_pot;
  
  for(;;) {
    // Atualiza os valores das variáveis locais
    xQueuePeek(fila_pos,&pos_pot,0);
    xQueuePeek(fila_ref,&ref,0);

    // Calcula o erro
    erro = ref - pos_pot;
    
    // Termo proporcional
    P = Kp*erro;
    
    // Termo integral
    Int += erro*dt;
    Int = constrain(Int,-Int_sat,Int_sat);
    I = Ki*Int;
    
    // Termo derivativo
    Der = (erro - erro_ant)/dt;
    D = Kd*Der;
    
    // Calcula o sinal PID
    PID = P + I + D;
    PID = constrain(PID,-((1 << res_PWM) - 1),((1 << res_PWM) - 1));
    
    // Aplica o sinal PID no motor
    if(PID > 0) {
      ledcWrite(canal_RPWM,abs(PID));
      ledcWrite(canal_LPWM,0);
    }
    else if(PID < 0) {
      ledcWrite(canal_RPWM,0);
      ledcWrite(canal_LPWM,abs(PID));
    }
    else {
      ledcWrite(canal_RPWM,0);
      ledcWrite(canal_LPWM,0);
    }
    
    // Guarda o erro
    erro_ant = erro;
    
    // Período da tarefa
    xTaskDelayUntil(&wake,pdMS_TO_TICKS(10));
  }
}

void Task_RotFisi(void *pvParameters) {}

void Task_LerCell(void *pvParameters) {}

void Task_Comunic(void *pvParameters) {}

// Define as funções
float map_float(float var,float de_min,float de_max,float para_min,float para_max) {
  return ((var - de_min)*(para_max - para_min)/(de_max - de_min) + para_min);
}

float ler_pot() {
  int leitura_ADC = analogRead(SIG_pot);
  float pos_pot = map_float(leitura_ADC,offset1_ADC,offset2_ADC,pos1_pot,pos2_pot);
  return constrain(pos_pot,pos1_pot,pos2_pot);
}