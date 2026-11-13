# Trabalho-de-Exoesqueleto

Básicos de github:

Como copiar o projeto e inicar ela?
-instale Git
-tem um icone no lado esquerdo onde tem as 3 bolinhas interligadas chamado source control, clica ela
-Nessa aba, tem do lado do changes, 3 pontinhos, clicando ela voce vai ver a opçao clone
-clicando ela, coloca esse link: https://github.com/erickimkhs/Trabalho-de-Exoesqueleto;

A Workflow:

-Se voce fez alguma mudança em algum file ou criou um novo, vai aparecer dentro do "Changes" a sua mudança; voce pode clicar "+" para salvar as mudanças. Como se voce colocou as coisas dentro uma caixa sem selar ela
-se clicou nesse "+", as mudanças estarão no "Staged changes", daí, pode clicar no "-" se mudou de ideia das mudanças e se realmente acha que as mudanças estão certas, Escreve um comentário das mudanças (ou clica nas estrelinhas para que o IA faça isso) e clica "Commit": fazendo isso voce tirou uma foto do estado atual do projeto, onde está salvo para voce voltar nela quando que quiser
-Envie para a nuvem (Push): Como o Commit fica salvo só no seu computador, clique no botão  Push para enviar suas "fotos" para o GitHub 
-Na mesma aba, tem o graph que vai te mostrar esse historico de commits

Outros comandos do source control:
-Na aba de source control, do lado um icone de 3 pontinhos, clicando ela, iremos ustilizar só esses 3:
-Pull: traz as mudanças que outras pessoas fizeram do github para a branch atual (explicarei em baixo) e já mescla essas mudanças com o seu codigo
-push: voce pega os commits da branch atual seu e envia para github
-fetch: ela pega as mudanças do github para  voce ver o que mudou, não tenta atualizar os codigos que tem no momento.
-Nota que do lado do graph tambem tem isso

Branches:
-Ela é uma linha do tempo onde tem a "main", a principar, e os "branches" que servem como linha do tempo paralelo
-ela funciona assim, no estado atual do codigo está estável, mas voce precisa adicionar alguns códigos que provavelmente vai quebrar tudo. Você vai criar uma branch para voce fazer qualquer commits que quiser e se tudo der errado, é só voltar para a main. Se deu certo, voce pode fundir a main com o seu branch
-como criar a branch: no canto inferior esquerdo, aparece em qual branch voce está, clicando nele voce pode criar sua branch com o nome que quer. Nota-se que as branches são só locais. (o jeito mais simples é ir no source control, clica nos 3 pontinhos do lado do "changes" e clica opçao "branch" e cria ela lá)
-mudar de linha de tempo: clica no mesmo icone e aparece as branch e voce pode escolher qual branch que voce quer trabalhar
-nota-se que pode fazer uma branch da sua branch com "create new branch from" onde voce usa quando voce ta incerto sobre o estado atual da branch mas tem medo de que vai quebrar tudo
-quando voce faz push, voce vai criar essa branch no github e colocar as commits lá. O jeito mais facil que achei foi no source contro,na parte do "graph", clica no push (icone da seta para cima)
-para deletar ela vai no source control e clica no 3 pontinhos e vai na opçao branch e deleta ela
-para juntar elas, fique na branch onde voce quer que as mudanças ocorram, vai na mesma aba de 3 pontinhos e clica merge e escolhe a branch que quer pegar as mudanças (não deleta a branch)

O que fazer no github:
-Vai no github, vai na aba de pull request, escolhe a origem e o destino e clica no botao "create pull request" fazer o pull request onde voce literalmente fala: minhas mudanaças são objetivamente superior a main e demando que as mudanças entrem na main. 

Finalmente agora que virou genius de git (trust):
1. A regra de ouro (Sempre dê Pull antes de começar): Antes de escrever qualquer linha de código no dia, clique em Pull (especialmente se estiver na main). Isso garante que você está trabalhando em cima da versão mais recente que seus "aliados" enviaram.

2. Como desfazer erros antes do Commit (Discard Changes): Se alguém mexeu em um arquivo, quebrou tudo e quer voltar exatamente para como estava na última "foto" (commit), basta passar o mouse em cima do arquivo dentro de Changes e clicar no ícone da setinha curvada para trás (Discard Changes).

3. Conflitos de Merge (Merge Conflicts): Vale avisar a equipe para não entrar em pânico se isso acontecer. Se duas pessoas editarem a mesma linha do mesmo arquivo e tentarem juntar o código (Pull ou Merge), o VS Code avisa que houve conflito e mostra botões em cima do código para você escolher o que manter:
-Accept Current Change: mantém o seu código.
-Accept Incoming Change: mantém o código que veio do GitHub/outra branch.
-Accept Both Changes: mantém os dois, um abaixo do outro.

5. NUNCA mexe na main, sempre trabalha no branch seu, e faz push desse branch, pls
