# Viabilidade e Plano de Implementação - SkateMod+ 

## Objetivo
Melhorar o Skate Mod existente, portar para ASI, adicionar animações do Tony Hawk's American Wasteland, e integrar com Urbanize (onde o skate mod já opera). Adicionar manobras, físicas de skate, grinds e melhorias viáveis.

## Estado Atual

### Skate Mod Existente (Analisado)
- **Tipo**: Baseado em veículo (skateboard montado como veículo ID 74)
- **Implementação**: Script CLEO (`cleo/skateboard.cs`)
- **Ativação**: Quando jogador tem arma configurada (padrão WeaponType 15, WeaponModel 326), pressiona tecla para montar skate
- **Animações**: Usa `CHOPPA_sprint` do IFP `CHOPPA`
- **Áudio**: `cleo_audio/skate.mp3` via audiostream
- **Config**: `cleo/skateboard.ini` (WeaponType, WeaponModel, VehicleID, Volume, ChangePickups)
- **Modelo**: `sktbd.dff/txd`, handling SKTBD
- **Pickups**: Criados em locais específicos se habilitado
- **Integração Urbanize**: Confirmada - `Urbanize/LS/GLNPS.cs` lê config do skate mod e referencia `sktbd`

### Urbanize (Analisado)
- **Tipo**: Mod grande com CLEO + ASI (`Urbanize (Junior_Djjr).asi`)
- **Dados**: `Urbanize.dat` controla spawns por área
- **Integração skate**: 
  - Flag `SKATER` (0x40000000) definido
  - `GLNPS.cs` verifica skate mod (`sktbd`, lê `cleo/skateboard.ini`)
  - Comportamentos PED existentes (BEGA1, LEAN1) para ambientação
- **Animações**: `djjr_urbanize.ifp` com animações próprias

### THAW (Analisado com REA)
- **Local**: `E:\downloads\Tony-Hawk-s-American-Wasteland_Win_EN_Full-Rip\THAW\Game\`
- **Executável**: `THAW.exe` (21MB) - jogo completo
- **Animações**: `data/anims/standardkeyQ.bin`, `standardkeyT.bin` (2048 bytes)
- **Scripts**: `data/scripts/engine/engine_params.qb.wpc`
- **Tricks encontrados**: ollie, kickflip, heelflip, grind, manual, revert, nollie (referenciados em textos)
- **Partymod**: Melhora input handling

## Viabilidade

### Alta Viabilidade
- **Port para ASI**: Viável - precisamos reimplementar lógica CLEO em plugin C++ usando Plugin SDK
- **Melhorias básicas**: Trapaças físicas, melhor controle, animações
- **Grinds/Manuals**: Sistema baseado em detecção de superfície - viável
- **Integração Urbanize**: Já existe, precisa manter compatibilidade (ler mesma config, usar mesmos modelos/IDs quando possível)
- **Adicionar animações THAW**: Precisa extrair/converter animações THAW para formato compatível com SA

### Média/Desafiadora
- **Sistema completo de tricks**: Requer muito mais que o atual (combo system, balance meters, special tricks) - pode ser complexo mas factível incrementalmente
- **Física avançada**: Motor físico do SA é limitado; imitar física THAW exatamente é difícil
- **Extração de animações THAW**: Formato binário proprietário precisa ser entendido - pode requerer ferramentas ou análise mais profunda
- **Revert/Vert**: Depende de detecção de vert ramps/quarter pipes

### Recomendações
- **Fazer incrementalmente**: Começar com melhorias no sistema atual (melhor física, mais animações) antes de sistema completo de tricks
- **Manter compatibilidade**: Não quebrar integração com Urbanize
- **Focar no viável primeiro**: Melhorias na montada/desmontada, física de direção, grinds básicos são mais alcançáveis

## Plano de Implementação

### Fase 1: Preparação e Estrutura ASI
1. **Criar estrutura do projeto ASI** em `E:/projetos/SkateMod+/src/`
   - Usar Plugin SDK para SA
   - Configuração similar ao skate mod atual
   - Manter compatibilidade com `cleo/skateboard.ini`
   
2. **Portar funcionalidades básicas**
   - Montar/desmontar skate (baseado em arma/tecla)
   - Criação do veículo sktbd
   - Animações básicas
   - Áudio
   - Sistema de pickups (opcional/configurável)

### Fase 2: Melhorias de Física e Controle
1. **Melhorar física**
   - Ajustar handling do skate
   - Melhor controle em curvas
   - Física de impulso/velocidade
   - Correções de câmera

2. **Sistema de balance**
   - Balance para manuals
   - Detecção básica de superfície

### Fase 3: Animações THAW e Tricks
1. **Integrar animações THAW**
   - Tentar extrair/converter animações de `standardkeyQ.bin/T.bin` ou de outros arquivos THAW
   - Adicionar animações para tricks básicos (ollie, flip tricks)
   - Usar animações do Urbanize quando apropriado ou adicionar novas

2. **Sistema de tricks básico**
   - Ollie
   - Flip tricks (kickflip, heelflip)
   - Grinds em corrimãos/edges (detecção)
   - Manuals
   - Combo básico

### Fase 4: Integração Urbanize e Polimento
1. **Garantir integração com Urbanize**
   - GLNPS.cs já lê config - manter compatível
   - Verificar se Urbanize spawn NPCs skatistas funciona
   - Testar em áreas integradas
   
2. **Polimento e testes**
   - Ajustes de física
   - Correção de bugs
   - Documentação atualizada
   - Commits regulares

## Tecnologias e Ferramentas Necessárias

- **Plugin SDK**: Para desenvolvimento ASI em C++
- **GTA SA modding knowledge**: Handling, vehicles, animations, memory
- **Ferramentas de análise**: REA (usado), possivelmente ferramentas para extrair .ifp
- **THAW assets**: Animações e possivelmente dados de física para referência

## Considerações

### Vantagens
- Base sólida já existe e funciona
- Integração com Urbanize já estabelecida
- Temos acesso completo aos arquivos de ambos os mods para análise
- Documentação extensa já criada

### Desafios
- Animações THAW em formato proprietário - extração pode ser difícil
- Física complexa do THAW pode não ser facilmente replicável
- Precisa manter compatibilidade com sistema existente
- Script CLEO original é compilado - tivemos que fazer engenharia reversa por strings

## Conclusão

O projeto é **viável** com abordagem incremental. Não tentar replicar THAW 100% logo de início, mas adicionar melhorias progressivas: primeiro port para ASI com melhorias na física/controle, depois adicionar animações THAW quando possível, e implementar sistema de tricks básico.

A integração com Urbanize já existe e deve ser preservada. O trabalho de documentação e engenharia reversa já realizado nos dá uma base sólida para implementação.

**Recomendação**: Começar pela Fase 1 (estrutura ASI) mantendo total compatibilidade com o skate mod atual e integração Urbanize existente.
