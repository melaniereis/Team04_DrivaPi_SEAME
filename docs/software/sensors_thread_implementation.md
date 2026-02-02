# Implementação do Sistema de Sensores com Threads

## Data: 28 de Janeiro de 2026

## Resumo
Criada a infraestrutura para gerenciamento de sensores via threads no firmware STM32 usando ThreadX RTOS. A implementação começa com o sensor HTS221 (temperatura/humidade) e está preparada para adicionar mais sensores.

## Arquitetura

### Estrutura de Ficheiros Criados

1. **`Core/Inc/sensors.h`** - Header com definições e protótipos
2. **`Core/Src/sensors.c`** - Implementação das threads de sensores

### Componentes Principais

#### 1. Estrutura de Dados do Sensor
```c
typedef struct {
    float temperature;
    float humidity;
    uint32_t timestamp;
    uint8_t data_valid;
} HTS221_Data_t;
```

#### 2. Proteção de Dados Partilhados
- **Mutex**: `g_sensorDataMutex` - Protege o acesso aos dados do sensor
- **Dados Globais**: `g_hts221_data` - Estrutura com leituras do sensor

#### 3. Thread do Sensor HTS221
```c
void SensorHTS221Thread(ULONG initial_input)
```
**Características:**
- Lê temperatura e humidade a cada 1 segundo (100 ticks)
- Usa mutex para proteger dados partilhados
- Marca dados como válidos/inválidos em caso de erro
- Debug output a cada 10 leituras (10 segundos)

#### 4. Função de Inicialização
```c
void SensorsInit(void)
```
**Responsabilidades:**
- Cria o mutex de proteção de dados
- Inicializa estruturas de dados
- Inicializa o hardware do sensor HTS221

## Integração no Sistema Existente

### Modificações em `app_threadx.h`
1. Adicionado `#include "sensors.h"`
2. Adicionado `sensor_hts221_e` ao enum `t_e_threads`
3. Aumentado array de threads de 6 para 7: `g_threads[7]`

### Modificações em `app_threadx.c`
1. Substituída inicialização inline do HTS221 por `SensorsInit()`
2. Mantida ordem de inicialização:
   - PCA9685 → Sensores → Threads

### Modificações em `thread_init.c`
1. Adicionada criação da thread HTS221 com:
   - **Nome**: "HTS221"
   - **Função**: `SensorHTS221Thread`
   - **Prioridade**: 8 (baixa prioridade, leituras periódicas)
   - **Stack**: 1024 bytes
   - **Auto-start**: Sim

## Prioridades das Threads

| Thread          | Prioridade | Razão                           |
|-----------------|------------|----------------------------------|
| CAN RX          | 2          | Receção de comandos crítica     |
| DC Motor        | 4          | Controlo de velocidade urgente  |
| Servo Motor     | 5          | Direção importante              |
| Speed Sensor    | 6          | Medição de velocidade           |
| CAN TX          | 7          | Transmissão de dados            |
| **HTS221**      | **8**      | **Telemetria não crítica**      |
| Supervisor      | 10         | Monitorização de baixa prioridade|

## Características da Implementação

### ✅ Vantagens
1. **Modular**: Cada sensor tem a sua própria thread
2. **Thread-safe**: Uso de mutex para dados partilhados
3. **Escalável**: Fácil adicionar novos sensores
4. **Robusto**: Tratamento de erros e validação de dados
5. **Debug-friendly**: Outputs informativos para desenvolvimento

### 🎯 Próximos Passos

#### Para Adicionar Mais Sensores:
1. Adicionar estrutura de dados em `sensors.h`
2. Criar função de thread em `sensors.c`
3. Adicionar enum em `app_threadx.h`
4. Criar thread em `thread_init.c`
5. Inicializar hardware em `SensorsInit()`

#### Para Enviar para Qt HMI:
1. **Via CAN**: Adicionar mensagens CAN com dados de sensores
2. **Via UART/USB**: Serializar dados em formato JSON ou binário
3. **Periodicidade**: Configurar taxa de transmissão apropriada

### 📊 Acesso aos Dados do Sensor

Para ler os dados do sensor de outras threads:

```c
float temp, hum;
uint8_t valid;

if (tx_mutex_get(&g_sensorDataMutex, TX_WAIT_FOREVER) == TX_SUCCESS)
{
    temp = g_hts221_data.temperature;
    hum = g_hts221_data.humidity;
    valid = g_hts221_data.data_valid;
    
    tx_mutex_put(&g_sensorDataMutex);
}

if (valid)
{
    // Usar temp e hum
}
```

## Verificação e Testes

### Como Testar:
1. **Compilar o projeto** para verificar erros de sintaxe
2. **Flash no STM32** e conectar UART
3. **Observar outputs**:
   - Inicialização do sensor
   - Leituras periódicas a cada 10 segundos
   - Detecção de erros (se houver)

### Outputs Esperados:
```
=== DrivaPi ThreadX Init ===
Initializing PCA9685 devices...
Initializing sensor subsystem...
Sensors: Initializing HTS221...
HTS221: Device detected (WHO_AM_I = 0xBC)
[...]
HTS221: Initialized successfully
Sensors: HTS221 initialized successfully
Initializing threads...
HTS221 Thread: Started
HTS221: T=23.45°C, H=45.2%, Count=10
HTS221: T=23.46°C, H=45.1%, Count=20
```

## Notas Importantes

1. **Período de Leitura**: 1 segundo (pode ser ajustado em `tx_thread_sleep(100)`)
2. **Stack Size**: 1024 bytes por thread (ajustar se necessário)
3. **Mutex Timeout**: `TX_WAIT_FOREVER` - considera `TX_NO_WAIT` para threads críticas
4. **Validação de Dados**: Sempre verificar `data_valid` antes de usar

## Conclusão

O sistema está pronto para coletar dados de telemetria do sensor HTS221 de forma contínua e thread-safe. A arquitetura permite fácil expansão para incluir mais sensores (acelerómetro, giroscópio, GPS, etc.) seguindo o mesmo padrão.

**Status**: ✅ Implementação completa e pronta para teste
