# SkateMod+ 

Projeto para melhorar o Skate Mod existente, portar para ASI, adicionar animações do Tony Hawk's American Wasteland e integrar com Urbanize.

## Objetivo
- Melhorar o Skate Mod já existente
- Portar para ASI
- Colocar as animações do Tony Hawks American Wasteland no mod
- Integrar juntamente ao Urbanize (onde o skate mod já opera originalmente)

## Estrutura

### Mods analisados
- **Urbanize** (`SA_-_Urbanize_12-25_Final_1/`) - Mod de urbanização para GTA San Andreas por Junior_Djjr
- **Skate Mod** (`Skate Mod/`) - Skateboard mod existente

### Documentação
Toda a documentação está em `docs/` com análise reversa detalhada:

- `urbanize-overview.md` - Visão geral do Urbanize
- `skatemod-overview.md` - Visão geral do Skate Mod existente
- `skatemod-reverse.md` - Engenharia reversa detalhada
- `skatemod-audio-anim.md` - Áudio e animações
- `integration-notes.md` - Notas de integração
- `urbanize-skate-integration.md` - Integração Urbanize + Skate Mod
- `glnps-analysis.md` - Análise do script GLNPS.cs (ponto de integração)
- `viability-assessment.md` - Avaliação de viabilidade
- `plan-next-steps.md` - Próximos passos
- `technical-summary.md` - Resumo técnico

## Pontos-chave descobertos

### Integração Urbanize + Skate Mod
- `Urbanize/LS/GLNPS.cs` faz referência ao `sktbd` e lê `cleo/skateboard.ini` (WeaponType/WeaponModel)
- Urbanize define flag `SKATER` (0x40000000) em `Urbanize.dat`
- Integração já existe - Urbanize "sabe" do Skate Mod e o suporta

### Skate Mod atual
- Implementação baseada em veículo (veículo ID 74, modelo sktbd)
- Ativado com arma/tecla quando não dirigindo
- Usa animações CHOPPA (`CHOPPA_sprint`)
- Áudio via audiostream
- Pickups configuráveis
- Config em `cleo/skateboard.ini`

## Próximos passos
1. Engenharia reversa mais profunda do script do Skate Mod
2. Planejamento da porta para ASI
3. Obtenção/integração das animações do Tony Hawk's American Wasteland
4. Garantir compatibilidade com integração Urbanize
5. Implementação e testes

## Commits
Todo o trabalho de documentação e análise está sendo commitado neste repositório.
