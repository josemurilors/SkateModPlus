# Especificação Completa de Engenharia Reversa - Skate Mod Original

Baseado na descompilação integral de `cleo/skateboard.cs`, na análise do `bikearmfix.asi` e na integração com o Urbanize (`GLNPS.cs`).
Créditos originais do conceito e mod: **Linevariy**, **Junior_Djjr**, **M. Osborn** (MixMods).

---

## 1. Arquitetura e Contratos de Dados

### 1.1 Modelos e IDs
- **Arma / Item Carregado**:
  - Slot: Arma branca / melee (substitui a bengala / `gun_cane`).
  - `WeaponType`: `15` (WEAPON_CANE)
  - `WeaponModel`: `326` (`gun_cane.dff` / `gun_cane.txd`)
- **Veículo / Skate no Chão**:
  - Modelo: `sktbd.dff` / `sktbd.txd`
  - `VehicleID`: `74` (adicionado via `Vehicles.ide` como classe `bicycle` / `bmx`)
  - Handling: `SKTBD` (Handling.cfg, baseado em bicicleta com centro de massa baixo e parâmetros de bmx)
- **Animações**:
  - Pacote: `choppa.ifp` (grupo de animações `CHOPPA`)
  - Animação base de movimento: `CHOPPA_sprint` (framedelta 4.0, sem loop rígido, reproduzida continuamente durante o controle)
- **Áudio**:
  - Arquivo: `cleo/cleo_audio/skate.mp3` (loop de rolagem no asfalto)

### 1.2 Configuração (`cleo/skateboard.ini`)
```ini
[Settings]
WeaponType    = 15   # Tipo da arma melee
WeaponModel   = 326  # Modelo da arma melee
VehicleID     = 74   # ID do veículo do skate
Volume        = 0.3  # Volume multiplicador (0.0 a 1.0)
ChangePickups = 1    # Se cria os 17 pickups pelo mapa de SA
```

---

## 2. Máquina de Estados do Mod Original (175 Opcodes)

### Fase A: Inicialização e Pickups
1. Aguarda 500 ms na inicialização do jogo.
2. Lê as configurações do `skateboard.ini`.
3. Se `ChangePickups == 1`:
   - Verifica se o pickup `$12824` já foi criado.
   - Se já existirem pickups anteriores, destroi os identificadores salvos.
   - Cria 17 pickups de arma do modelo `WeaponModel` (326) e tipo `WeaponType` (15) em pontos estratégicos de San Andreas:
     - Glen Park Skatepark: `(1868.60, -1363.30, 13.52)`
     - Vinewood / Temple: `(1307.52, -801.73, 84.14)`
     - Ocean Docks / Willowfield: `(1660.56, -2106.45, 13.57)`
     - East Los Santos: `(2164.03, -1988.37, 14.00)`
     - Las Colinas: `(2606.06, -1080.04, 69.64)`
     - Mount Chiliad: `(-2334.59, -1676.31, 483.45)`
     - Angel Pine / Flint County: `(-2540.82, -623.00, 132.83)`
     - San Fierro (Esplanada): `(-2569.12, 983.29, 78.40)`
     - San Fierro (Garcia Skatepark): `(-2116.11, 926.84, 86.16)`
     - Las Venturas (Primp): `(2629.39, 2418.32, 10.86)`
     - Montgomery / Red County: `(1323.57, 206.45, 19.53)`
     - East Beach: `(2536.27, -1665.41, 15.12)`
     - Rodeo / Richman: `(947.60, -721.87, 122.16)`
     - Santa Maria Beach: `(400.49, -2062.92, 10.71)`
     - Marina: `(1207.94, -2070.64, 69.06)`
     - Los Santos Downtown: `(1359.12, -1718.81, 8.70)`
     - San Fierro Hill: `(-1267.24, 966.04, 133.05)`

### Fase B: Condições de Montaria (Mount Trigger)
O jogador pode montar no skate se todas as condições forem atendidas:
1. Jogador não está morto (`!pPlayer->IsDead()`).
2. Jogador não está em nenhum veículo (`pPlayer->m_pVehicle == nullptr`).
3. Tecla pressionada: Tecla `4` de controle do jogo (`KEY_SECONDARY_ATTACK` / `TAB` no teclado / Look Behind / Action).
4. Arma atual é a `WeaponType` (15).
5. Jogador está controlável (`pPlayer->m_nPedState != PEDSTATE_ARRESTED`, etc.).
6. Jogador não está no ar (`!pPlayer->bIsInAir`).
7. **Verificação de Obstáculo e Chão**:
   - Calcula posição 0.3m à frente do jogador e pega a altura do solo (`CWorld::FindGroundZForCoord`).
   - Calcula posição 3.0m à frente do jogador e pega a altura do solo.
   - Testa linha de visão sem colisão sólida (`no_obstacles_between` / Raycast de colisão com sólidos, carros, pedestres e objetos).

### Fase C: Spawn e Transição para o Skate
1. Salva a posição `(X, Y, Z)` e o ângulo `Heading` do jogador.
2. Requisita e carrega o modelo do veículo `VehicleID` (74).
3. Spawna o veículo `sktbd` na posição do jogador e aplica o mesmo `Heading`.
4. Vincula o veículo ao mesmo interior ativo do jogador (`link_car_to_interior`).
5. Coloca o jogador dentro do veículo (`WARP_CHAR_INTO_CAR` / `CPlayerPed::SetPedPositionInCar`).
6. Ajusta a altura Z para o solo + 0.2m.
7. Dispara a animação `CHOPPA_sprint` do pacote `CHOPPA`.
8. Aplica velocidade inicial para frente: `speed = 7.0` (impulso inicial de saída).
9. Inicia o áudio:
   - Carrega `cleo/cleo_audio/skate.mp3` como stream em loop.
   - Inicializa com volume 0.0 e dá Play.
10. **Truque visual do Skate na Mão**:
    - Esconde todas as armas temporariamente (`set_actor_all_weapons_hidden 1`).
    - Altera a arma equipada para punho (`armed_weapon = 0`).
    - Re-habilita armas (`set_actor_all_weapons_hidden 0`), garantindo que o CJ não apareça segurando o skate nas mãos enquanto pilota.

### Fase D: Loop de Pilotagem e Áudio Dinâmico
A cada frame em que o jogador está no skate:
1. Garante que nenhuma arma fique empunhada (força punho se ainda estiver selecionando o skate).
2. Se o carro foi destruído ou o jogador foi ejetado (`actor_car != skate_car`):
   - Para e destrói a stream de áudio.
   - Destrói a instância do veículo.
   - Recarrega e re-equipa a arma `gun_cane` (skate na mão) para o jogador.
   - Retorna ao estado inicial.
3. Se o skate está no chão e em movimento (`!car_in_air && !car_stopped`):
   - Calcula distância entre a câmera ativa e o skate: `dist = distance(skate, cam) / 2.0`.
   - Limita `dist` mínimo em 1.0.
   - Fator de atenuação por distância: `dist_vol = 1.0 / dist`.
   - Fator de velocidade: `speed_vol = clamp(car_speed / 20.0, 0.0, 1.0)`.
   - Lê volume master SFX do GTA SA: endereço de memória `0xB5FCE4` (`CAudioEngine::m_nSfxVolume`).
   - Volume final do áudio = `dist_vol * speed_vol * (master_vol / 64.0) * ini_volume`.
   - Atualiza o volume da stream de áudio continuamente.
4. Se o skate estiver no ar ou parado:
   - Volume é zerado imediatamente (`volume = 0.0`).

---

## 3. Análise do `bikearmfix.asi` (Correção dos Braços)
- **Problema no GTA SA**: Veículos da classe `bicycle` acionam uma rotina interna em `CPedIK` (`CPedIK::PointGunAtPosition` e cálculo de articulação para o guidão), que puxa os nós dos braços do modelo humano em direção às coordenadas fixas do guidão do veículo.
- **Solução do `bikearmfix.asi`**: Desativa ou anula o ajuste dos braços quando a classe do veículo é bicicleta ou quando uma animação customizada está tocando, permitindo que a pose livre de skatista (`choppa.ifp`) mantenha as mãos e braços naturais.
- **Implementação no SkateMod+**: Incorporar esse patch diretamente via injetor/hooks de memória no `.asi`, tornando o mod 100% autossuficiente sem precisar de outro `.asi` na pasta.

---

## 4. Integração com o Urbanize (`GLNPS.cs`)
O script `Urbanize/LS/GLNPS.cs` funciona da seguinte forma:
1. Executa `get_model_by_name "sktbd"` para obter o ID do modelo.
2. Se não encontrar `sktbd`, utiliza a BMX (ID 481) como fallback.
3. Lê `CLEO\skateboard.ini`:
   - Chave `WeaponType`
   - Chave `WeaponModel`
4. Se existirem, os pedestres skatistas gerados pelo Urbanize em Glen Park recebem a arma do skate e pilotam usando a mesma configuração.
5. **Conclusão**: O `SkateMod+.asi` deve ler e respeitar o arquivo `cleo/skateboard.ini` (e criá-lo com valores padrão caso não exista) para que o Urbanize continue funcionando perfeitamente sem qualquer modificação.
