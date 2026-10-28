# Compatibilidade com SAAWR e Urbanize - SkateMod+

Data: 2026-10-27

## 1) Objetivo
Verificar viabilidade de adaptar **apenas o Skate Mod** para funcionar em conjunto com:
- **Urbanize** (já integrado, **não mexer**)
- **SAAWR** (Add Without Replace, gerencia IDs/FLA/ModLoader)

## 2) Como o SAAWR funciona (analisado)
Baseado em `E:/projetos/SAAWR/crates/awr-core/`:

- **Alocação de IDs** (`ids.rs`):
  - Veículos: faixa `12500..=19999`, pula `12400..=12499` (reservado a armas). Pega **menor ID livre**.
  - Armas: model ID em `12400..=12499`, weapon type ID começa em `70`.
  - `collect_used_ids()` considera: IDs originais do jogo + **qualquer pasta `{id}-*`** encontrada em `modloader/` (varre pastas top-level e um nível abaixo), tanto habilitadas quanto prefixadas com `. ` (desativadas). Ou seja: **qualquer mod com padrão `{id}-` conta como ID em uso**.
- **Inventário** (`inventory.rs`): só escaneia `modloader/awr-cars`, `modloader/awr-weapons`, `modloader/awr-roupas`. Pastas com `meta.json` válido viram mods gerenciados pelo SAAWR; pastas sem `meta.json` válido são tratadas como **"foreign"** (listadas mas **nunca tocadas** pelo SAAWR).
- **Injeção** (`injector.rs`): monta em `modloader/. awr-staging/`, edita globais (handling/ide/carcols/cargrp/áudio/FLA) com `.bak`, promove com rename atômico. Usa pastas próprias em `awr-cars/{ID}-{short}/` com `meta.json`.
- **FLA/OLA**: gerencia limites e aplica patches conforme necessário. OLA é esperado/obrigatório em muitos fluxos.

## 3) Como o Skate Mod atual funciona
Baseado em `C:/Users/josen/OneDrive/old/Área de Trabalho/Skate Mod/` e docs do SkateMod+:

- **Tipo**: CLEO-based, veículo montado como **veículo ID 74** (config em `cleo/skateboard.ini` -> `VehicleID = 74`)
- **Ativação**: via arma configurada (`WeaponType/WeaponModel`), tecla para montar
- **Modelo**: `sktbd.dff/txd` (mencionado em `Linhas.txt`, `cleo_text/skateboard.fxt`)
- **Animações**: `CHOPPA_sprint` via `choppa.ifp`
- **Áudio**: `cleo/cleo_audio/skate.mp3` via audiostream
- **Integração Urbanize**: `Urbanize/LS/GLNPS.cs` lê `CLEO\skateboard.ini` e referencia `sktbd`. **Deve permanecer inalterado.**

## 4) Análise de Conflitos

| Ponto | Risco | Explicação |
|---|---|---|
| **ID do veículo (74)** | **Alto** | Skate Mod usa ID 74. SAAWR considera IDs em pastas `{id}-*` como "em uso". Se alguém colocar `74-sktbd` ou similar em `modloader/` seguindo padrão SAAWR, ou se SAAWR alocar perto disso, há chance de colisão. Também `collect_used_ids` vê pastas com padrão numérico. |
| **Localização** | **Médio-Alto** | Atualmente vive em `cleo/` (script + ini + áudio). SAAWR gerencia mods em `modloader/awr-cars/`. ModLoader carrega tudo, mas SAAWR **só gerencia** `awr-cars/`, `awr-weapons/`, `awr-roupas/`. Um mod "solto" fora dessas pastas é **foreign** pro SAAWR e **não será tocado** por injeção/remoção/edição do SAAWR. |
| **Urbanize lê `cleo/skateboard.ini`** | **Crítico (não mexer)** | GLNPS.cs faz `get_int_from_ini_file "cleo\skateboard.ini"...` - **caminho e nome devem permanecer**. Qualquer migração para `modloader/awr-cars/...` precisa manter esse arquivo acessível em `cleo/skateboard.ini` OU garantir que Urbanize continue lendo do mesmo lugar. |
| **FLA sync** | **Médio** | Se SAAWR sincronizar FLA e gerenciar limites, um veículo "não gerenciado" por SAAWR pode não ter suas entradas tratadas pelo fluxo do SAAWR, mas ModLoader/FLA ainda podem funcionar. Precisa testar. |
| **Referências `sktbd`** | **Baixo-Médio** | Urbanize e possivelmente outros scripts referenciam `sktbd`. Nome do modelo deve permanecer compatível. |

## 5) Viabilidade

**Viável, com adaptações apenas no Skate Mod.**

Não é necessário mexer no Urbanize. O ponto-chave é **manter compatibilidade retroativa**:
- Manter `cleo/skateboard.ini` funcional (Urbanize depende)
- Evitar que SAAWR "confunda" ou sobrescreva o skate mod
- Tornar o skate mod menos propenso a conflitos de ID com alocação do SAAWR

## 6) Plano de Adaptação (apenas Skate Mod)

### Abordagem A: "Compatível sem migração" (mais segura, menos invasiva)
Ideal para coexistência imediata:

1. **Documentar e isolar**: Garantir que o skate mod não siga o padrão `{ID}-` em pastas que o SAAWR escaneia agressivamente. Se ficar em `cleo/` puro, SAAWR não o gerencia (foreign) e não tenta injetar/remover.
2. **Tornar ID configurável e "seguros"**: Manter `VehicleID` configurável no ini. Sugerir ao usuário usar ID fora de faixas que SAAWR aloca tipicamente ou IDs que não conflitem. Documentar.
3. **Preservar `cleo/skateboard.ini`**: Nunca mover/remover. Criar cópia/fallback se necessário, mas manter caminho original.
4. **Verificar com SAAWR**: Testar que SAAWR não quebra ao ter o skate mod instalado fora de `awr-cars/`.

### Abordagem B: "Adaptado ao estilo SAAWR" (melhor integração a longo prazo)
Mais "correto" mas requer mais cuidado:

1. **Criar estrutura opcional em `modloader/awr-cars/`**: Ex: `modloader/awr-cars/74-sktbd/` ou usar ID alocado pelo SAAWR? 
   - **Problema**: Se migrar para ser gerenciado pelo SAAWR, o SAAWR pode querer controlar handling/ide/cargrp/áudio via seu injector. Mas o skate mod atual é CLEO-based com lógica própria.
   - **Solução**: Criar uma versão "SAAWR-friendly" que **não** conflite com fluxo do SAAWR. Ou adicionar `meta.json` minimalista? Cuidado - se tiver `meta.json` válido e estiver em `awr-cars/`, SAAWR o trata como gerenciado.
2. **Manter caminho do ini**: Se migrar, garantir que `cleo/skateboard.ini` continue existindo (pode ser copiado/symlinkado ou mantido em local original).
3. **Evitar colisão de IDs**: Se SAAWR for usado, preferir deixar o skate mod com ID fora do range que será alocado, ou coordenar. Mas SAAWR aloca dinamicamente.

**Recomendação:** Começar pela **Abordagem A**. É mais segura e não toca Urbanize. Depois evoluir para B se necessário.

## 7) Ações Imediatas (no SkateMod+)

1. **Criar documentação** (este arquivo) - feito
2. **Adicionar verificação de compatibilidade**: Criar script/checklist para testar coexistência
3. **Tornar configuração mais robusta**: Garantir que `cleo/skateboard.ini` seja lido corretamente mesmo com outros mods
4. **Documentar para usuário**: Explicar como instalar junto com SAAWR
5. **Commitar tudo**

## 8) Checklist de Testes

- [ ] Instalar Urbanize + Skate Mod original - funciona (baseline)
- [ ] Instalar SAAWR + Skate Mod fora de `awr-cars/` (em `cleo/`) - verificar se SAAWR não quebra nada
- [ ] Verificar se `collect_used_ids` do SAAWR não conflita com pasta solta
- [ ] Testar injeção de carro via SAAWR enquanto skate mod existe
- [ ] Verificar GLNPS.cs ainda lê `cleo/skateboard.ini` corretamente
- [ ] Verificar que SAAWR não remove/modifica arquivos do skate mod

## 9) Conclusão

**Viável sem mexer no Urbanize.** 

O maior ponto de atenção é **não mover `cleo/skateboard.ini`** e **não fazer o skate mod ser gerenciado pelo SAAWR** acidentalmente. Mantendo o skate mod em sua estrutura atual (CLEO) fora de `modloader/awr-cars/`, ele deve coexistir como mod "foreign" para o SAAWR, enquanto continua funcionando normalmente com Urbanize.

A chave é **adaptar apenas o Skate Mod para ser "SAAWR-aware" sem ser "SAAWR-managed"**.
