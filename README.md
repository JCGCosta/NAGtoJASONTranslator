# NAGtoJASONTranslator
This repo contains a compiler, with lexical and syntactic analyzer for translating a simple language called NAG to JASON/AgentSpeak.

## Input example

```
bob1 crencas: { estaChovendo ; naotenhoGuardaChuva ; }
objetivos: { comprarGuardaChuva ; naoPegarChuva ; }
planos: { plano1 ( comprarGuardaChuva ; estaChovendo E naotenhoGuardaChuva ;{ sair; procurarLoja; comprarGuardaChuva; } ) ; }
```

## Output example

```
naotenhoGuardaChuva.
estaChovendo.

+comprarGuardaChuva : estaChovendo & naotenhoGuardaChuva
  <- !sair;
     !procurarLoja;
     !cagarNoPortao;
     !comprarGuardaChuva.
```

To get to that point i create a data structure to better fit to the problem. The graphic representation is showed below.

<img src="https://github.com/JCGCosta/NAGtoJASONTranslator/blob/main/src/datastructure.png?raw=true" alt="datastructure" title="datastructure">

* CA -> Agent head: is the starting point of te structure, who owns the agent name.
* LC -> Beliefs list: who owns the agent beliefs.
* LO -> Goals list: who owns the agent goals.
* LP -> Plans list: who owns the plans, each of those plans has 3 different structures: a list of plan triggers, the context and the plan execution body. 
