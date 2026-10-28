# SkateMod+

SkateMod+ é a evolução do Skate Mod original para GTA San Andreas. Portado para ASI, traz animações, tricks, grinds, manuals e física aprimorados inspirados em Tony Hawk's American Wasteland. Mantém a integração já existente com o Urbanize sem editar nenhum de seus arquivos, e foi projetado para coexistir com o SAAWR.

## Status
Este repositório contém apenas **documentação e análise reversa**. O código fonte ainda será implementado.

## Documentação
Toda a análise está em `docs/`:

- `VIABILITY_AND_IMPLEMENTATION_DISCUSSION.md` - Plano detalhado de implementação
- `saawr-compatibility.md` - Análise de compatibilidade com SAAWR
- `urbanize-skate-integration.md` - Pontos de integração com Urbanize
- `thaw-analysis.md` - Análise das animações e mecânicas do THAW
- `skatemod-reverse.md` - Engenharia reversa do Skate Mod original
- `glnps-analysis.md` - Análise da integração via GLNPS.cs
- E outros documentos de pesquisa

## Estratégia
- **Urbanize-aware, não Urbanize-modifying**: Respeita os contratos existentes (`cleo/skateboard.ini`, referências `sktbd`)
- **SAAWR-safe**: Projetado para coexistir sem ser gerenciado pelo SAAWR
- **Incremental**: Port para ASI mantendo compatibilidade com o mod original
