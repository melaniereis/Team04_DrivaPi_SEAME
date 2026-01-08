# ThreadX Codebase Analysis & Doxygen Documentation Guide

## 1 - Overview

This document provides a comprehensive analysis of the ThreadX real-time operating system implementation in the DrivaPi project. It details the thread structure, inter-thread communication mechanisms, and explains the Doxygen documentation generation process.

---

## 2 - ThreadX Codebase Analysis

### 2.1 Project Structure

The ThreadX implementation is located in `/Threadx` directory with the following structure:

```
Threadx/
├── Core/
│   ├── Inc/          # Header files
│   ├── Src/          # Source implementation files
│   ├── Startup/      # STM32 startup files
│   └── Test/         # Test files
├── AZURE_RTOS/       # Azure RTOS middleware
│   └── App/
├── Drivers/          # Hardware drivers
├── Middlewares/      # STM32 middlewares
├── Doxyfile          # Documentation configuration
└── STM32U585AIIXQ.*  # Linker scripts and device configuration
```

**Key Source Files:**
- `app_threadx.c/h` - Main ThreadX application initialization
- `thread_init.c` - Thread creation and initialization
- `supervisor.c` - Supervisor thread implementation
- `dc_motor.c` - DC motor control thread
- `servo_motor.c` - Servo motor control thread
- `speed_sensor.c` - Speed sensor thread
- `canRX.c` - CAN receive thread
- `canTX.c` - CAN transmit thread

---

## 3 - Main Entry Points and Initialization

### 3.1 Primary Entry Point: `App_ThreadX_Init()`

**File:** `Core/Src/app_threadx.c`

**Purpose:** Main initialization function for the entire ThreadX application

**Key Initialization Steps:**

1. **Hardware Device Initialization**
   - Initializes PCA9685 I2C PWM controllers: `PCA9685_Init_All_Devices()`
   - Stops all motors initially: `Motor_Stop()`

2. **Communication Structures Creation**
   - **Speed Command Queue:** `queue_speed_cmd` - Handles speed commands from CAN
   - **Steering Command Queue:** `queue_steer_cmd` - Handles steering commands from CAN
   - **Event Flags Group:** `event_flags` - System-wide event signaling
   - **Speed Data Mutex:** `speed_data_mutex` - Protects global speed variable

3. **Thread Initialization**
   - Calls `thread_init()` to create all application threads

### 3.2 Secondary Entry Point: `thread_init()`

**File:** `Core/Src/thread_init.c`

**Purpose:** Creates and configures all application threads

**Return Value:** Returns `TX_SUCCESS` or `TX_THREAD_ERROR` status

---

## 4 - Thread Creation and Priority Levels

All threads are created using the `tx_thread_create()` API with the following parameters:

```c
tx_thread_create(
    &thread_control_block,  // Thread control structure
    "thread_name",          // Descriptive name
    thread_entry_function,  // Entry point function
    initial_input,          // Input parameter (unused = 0)
    stack_pointer,          // Thread stack (1024 bytes)
    THREAD_STACK_SIZE,      // Stack size constant
    priority,               // Thread priority
    preemption_threshold,   // Preemption threshold (same as priority)
    TX_NO_TIME_SLICE,       // No time-slicing
    TX_AUTO_START           // Auto-start on creation
);
```

### 4.1 Thread Configuration Table

| Thread Name | Entry Function | Priority | Preemption | Status |
|------------|---|---|---|---|
| **Supervisor** | `ld1_thread_entry()` | 10 | 10 | AUTO_START |
| **DC Motor** | `dc_motor()` | 4 | 4 | AUTO_START |
| **Servo Motor** | `servo_motor()` | 5 | 5 | AUTO_START |
| **Speed Sensor** | `speed_sensor()` | 6 | 6 | AUTO_START |
| **CAN TX** | `canTX()` | 7 | 7 | AUTO_START |
| **CAN RX** | `canRX()` | 2 | 2 | AUTO_START |

**Priority Distribution:**
- **Highest Priority (P2):** CAN RX - Handles incoming CAN messages immediately
- **High Priority (P4-P5):** Motor Control - DC and Servo motor threads
- **Medium Priority (P6-P7):** Sensor and CAN TX
- **Lowest Priority (P10):** Supervisor - Low-priority monitoring thread (it will have a bigger priority when the code from this thread is implemented)

**Threading Model:**
- Total threads: 6
- Stack size per thread: 1024 bytes (1 KB)
- Time slicing: Disabled (TX_NO_TIME_SLICE)
- All threads auto-start upon creation

---

## 5 - Function Call Sequences and Dependencies

### 5.1 Initialization Call Flow

```
main()
  ↓
App_ThreadX_Init()
  ├── PCA9685_Init_All_Devices()
  ├── Motor_Stop()
  ├── tx_queue_create(&queue_speed_cmd)
  ├── tx_queue_create(&queue_steer_cmd)
  ├── tx_event_flags_create(&event_flags)
  ├── tx_mutex_create(&speed_data_mutex)
  ├── thread_init()
  │   ├── tx_thread_create(supervisor_e thread)
  │   ├── tx_thread_create(dc_motor_e thread)
  │   ├── tx_thread_create(servo_motor_e thread)
  │   ├── tx_thread_create(speed_sensor_e thread)
  │   ├── tx_thread_create(can_tx_e thread)
  │   └── tx_thread_create(can_rx_e thread)
  └── return TX_SUCCESS
```

### 5.2 Runtime Execution Flow

```
CAN RX Thread (Priority 2)
  ├── can_receive() - Poll CAN FIFO
  ├── Identify command type (CMD_SPEED = 44, CMD_STEERING = 45)
  ├── Send to appropriate queue
  └── Set event flag (FLAG_CAN_SPEED_CMD / FLAG_CAN_STEER_CMD)
       ↓
DC Motor / Servo Motor Threads (Priority 4-5)
  ├── Wait on queue/event flags
  ├── Process command
  ├── Update motor state via PCA9685
  └── Update global state
       ↓
Speed Sensor Thread (Priority 6)
  ├── Poll speed sensor hardware
  ├── Acquire speed_data_mutex
  ├── Update global speed variable
  └── Release mutex
        ↓
CAN TX Thread (Priority 7)
  ├── Read global vehicle speed
  ├── Format CAN message
  └── Transmit via CAN bus
       ↓
Supervisor Thread (Priority 10)
  └── Monitor system health via UART
```

---

## 6 - Inter-Thread Communication Mechanisms

### 6.1 Queues

**Definition:** FIFO message passing structures for command distribution

#### Queue: `queue_speed_cmd`
- **Purpose:** Speed command distribution from CAN
- **Data Type:** `t_can_message` (32 bytes)
- **Capacity:** 10 messages (QUEUE_SIZE = 10)
- **Producer:** CAN RX thread
- **Consumer:** DC Motor thread
- **Send Method:** `tx_queue_send()` with `TX_NO_WAIT` (non-blocking)
- **Receive Method:** DC motor waits/processes commands

#### Queue: `queue_steer_cmd`
- **Purpose:** Steering command distribution from CAN
- **Data Type:** `t_can_message` (32 bytes)
- **Capacity:** 10 messages
- **Producer:** CAN RX thread
- **Consumer:** Servo Motor thread
- **Usage:** Similar to speed queue

**CAN Message Structure:**
```c
typedef struct can_message_s
{
    uint32_t id;     // CAN message ID
    uint8_t  data[8]; // CAN data payload (8 bytes)
    uint8_t  len;    // Data length (1-8)
} t_can_message;
```

### 6.2 Event Flags

**Definition:** Lightweight synchronization primitive for signaling

**Event Flags Group:** `event_flags`

| Flag Name | Value | Purpose |
|-----------|-------|---------|
| `FLAG_CAN_SPEED_CMD` | (1 << 0) | Speed command received signal |
| `FLAG_CAN_STEER_CMD` | (1 << 1) | Steering command received signal |
| `FLAG_SENSOR_UPDATE` | (1 << 2) | Sensor update ready signal |

**Usage Pattern:**
```c
// Producer (CAN RX thread)
tx_queue_send(&queue_speed_cmd, &msg, TX_NO_WAIT);
tx_event_flags_set(&event_flags, FLAG_CAN_SPEED_CMD, TX_OR);

// Consumer (DC Motor thread)
tx_event_flags_get(&event_flags, FLAG_CAN_SPEED_CMD, 
                   TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
```

### 6.3 Mutex

**Definition:** Mutual exclusion lock preventing race conditions on shared data

**Mutex:** `speed_data_mutex`

- **Purpose:** Protects global speed variable `g_vehicle_speed`
- **Protected Resource:** `float g_vehicle_speed`
- **Acquiring Thread:** Speed sensor thread (reads/writes speed data)
- **Waiting Threads:** CAN TX thread (reads for transmission)
- **Timeout:** `TX_WAIT_FOREVER` (blocking wait)

**Protected Data Access Pattern:**
```c
// Speed sensor thread - WRITER
tx_mutex_get(&speed_data_mutex, TX_WAIT_FOREVER);
g_vehicle_speed = measured_speed;
tx_mutex_put(&speed_data_mutex);

// CAN TX thread - READER
tx_mutex_get(&speed_data_mutex, TX_WAIT_FOREVER);
float current_speed = g_vehicle_speed;
tx_mutex_put(&speed_data_mutex);
```

### 6.4 Communication Mechanism Summary

| Mechanism | Type | Use Case | Parties |
|-----------|------|----------|---------|
| **Queue (Speed)** | FIFO | Async command passing | CAN RX → DC Motor |
| **Queue (Steer)** | FIFO | Async command passing | CAN RX → Servo Motor |
| **Event Flags** | Signal | Lightweight notification | All threads |
| **Mutex (Speed)** | Lock | Critical section | Speed Sensor ↔ CAN TX |

---

## 7 - Doxyfile Configuration

### 7.1 Doxyfile Location and Purpose

**File Path:** `/Threadx/Doxyfile`

**Purpose:** Configuration file for Doxygen documentation generation system. Doxygen parses C/C++ source code and automatically generates comprehensive API documentation.

### 7.2 Critical Configuration Settings

#### Project Information
```
PROJECT_NAME           = "DrivaPi"
OUTPUT_LANGUAGE        = English
```

#### Input Configuration
```
INPUT                  = Core/Src \
                         Core/Inc \

RECURSIVE              = YES
FILE_PATTERNS          = *.c *.cc *.cxx *.cpp *.h *.hh *.hxx *.hpp (and many more)
```

**Explanation:**
- Processes all `.c` and `.h` files in `Core/Src` and `Core/Inc` directories
- Recursively searches subdirectories
- Automatically detects C/C++ language based on file extensions

#### Output Configuration
```
OUTPUT_DIRECTORY       = (current directory - generates html/ folder)
CREATE_SUBDIRS         = NO
EXTRACT_ALL            = YES
SOURCE_BROWSER         = NO
```

**Explanation:**
- Generates HTML documentation in `html/` folder by default
- `EXTRACT_ALL = YES` documents all entities, even undocumented ones
- `SOURCE_BROWSER = NO` does not include source code in output (reduces size)

#### Documentation Features
```
BRIEF_MEMBER_DESC      = YES      # Include brief function descriptions
REPEAT_BRIEF           = YES      # Repeat brief in detailed description
MARKDOWN_SUPPORT       = YES      # Enable Markdown in comments
AUTOLINK_SUPPORT       = YES      # Auto-link documented identifiers
```

#### Build Settings
```
GENERATE_TODOLIST      = YES      # Create @todo list
GENERATE_TESTLIST      = YES      # Create @test list
GENERATE_BUGLIST       = YES      # Create @bug list
GENERATE_DEPRECATEDLIST= YES      # Create @deprecated list
```
**Explanation:**
- Doxygen will parse the tags (@todo, @bug, etc.) and will create separate list pages for them.

#### Warning Configuration
```
WARNINGS               = YES
WARN_IF_UNDOCUMENTED   = YES      # Warn about undocumented symbols
WARN_IF_DOC_ERROR      = YES      # Warn about documentation errors
WARN_IF_INCOMPLETE_DOC = YES      # Warn about incomplete documentation
```

#### Performance
```
NUM_PROC_THREADS       = 1        # Single-threaded processing
LOOKUP_CACHE_SIZE      = 0        # Standard cache size
```

### 7.3 Key Doxyfile Features for ThreadX Project

1. **C Language Optimization**
   - `OPTIMIZE_OUTPUT_FOR_C = YES`
   - Proper handling of C structs and functions

2. **Cross-Referencing**
   - Automatic linking between functions, types, and files
   - Call graphs and caller graphs

3. **Documentation Standards**
   - Supports standard Doxygen comment styles:
     - `/// brief description`
     - `/** detailed description */`
     - `/** @brief ... @param ... @return ... */`

---

## 8 - Doxygen Documentation Workflow

### 8.1 Workflow File

**File Location:** `.github/workflows/doxygen_documentation.yml`

**Trigger Events:**
- Manual push to repository
- Pull requests

### 8.2 Workflow Execution Steps

#### Step 1: Checkout Repository
Clones the repository code into the CI/CD environment

#### Step 2: Install Doxygen and Dependencies
**Installed Tools:**
- **Doxygen:** Documentation generation engine
- **Graphviz:** Generates call graphs and dependency diagrams

#### Step 3: Generate Documentation
**Generated Output Structure:**
```
Threadx/html/
├── index.html              # Main documentation page
├── annotated.html          # Class/struct list
├── files.html              # File listing
├── functions.html          # Function reference
├── groups.html             # Module/group documentation
├── namespaces.html         # Namespace documentation (if C++)
├── pages.html              # Custom documentation pages
├── search/                 # Search functionality
├── classdocs/              # Class/struct documentation
└── ... (many more HTML files)
```

#### Step 4: Upload as Artifact
**Purpose:** Makes generated documentation downloadable from GitHub Actions

#### Step 5: Archive for Main Branch
**Condition:** Only on push to main branch
**Output:** Compressed tar.gz archive of entire HTML documentation

#### Step 6: Upload Release Artifact
**Purpose:** Long-term storage of documentation archives on main branch
**Retention:** 60 days (longer than regular artifacts)

---

## 9 - Doxygen Output Deliverables

### 9.1 Generated Documentation Types

#### 1. **HTML Documentation** (Primary Output)
- **Location:** `Threadx/html/`
- **Access:** Open `index.html` in web browser
- **Features:**
  - Full-text search capabilities
  - Cross-referenced links
  - Hierarchical navigation

#### 2. **API Reference**

**Files Documentation Page** (`files.html`)
- Lists all source files
- Shows file dependencies
- Displays #includes

**Functions Reference** (`functions.html`)
- Alphabetical function listing
- Function signatures
- Brief descriptions
- Linked to implementations

**Structs/Types Reference** (`annotated.html`)
- All struct definitions
- Member variables with types
- Inheritance hierarchy (if C++)

#### 3. **Advanced Diagrams**

**Call Graphs**
- Visual representation of function calls
- Shows function dependencies

**Caller Graphs**
- Shows which functions call a given function
- Reverse dependency visualization

**Dependency Diagrams**
- File inclusion relationships
- Module dependencies

---

## 10 - Best Practices for ThreadX Documentation

### 10.1 Improving Documentation Quality

**Add Doxygen Comments to Code:**
```c
/**
 * @brief Initialize all ThreadX resources and threads
 * @param memory_ptr Pointer to allocated memory for queues
 * @return TX_SUCCESS on successful initialization, TX_THREAD_ERROR on failure
 * 
 * @details
 * This function performs the following operations:
 * - Initializes PCA9685 PWM controllers
 * - Creates message queues for CAN commands
 * - Creates synchronization primitives (event flags, mutex)
 * - Creates and starts all application threads
 * 
 * @see thread_init(), tx_thread_create()
 */
UINT App_ThreadX_Init(VOID *memory_ptr)
```

**Document Thread Functions:**
```c
/**
 * @brief CAN message receiver thread
 * @param initial_input Unused parameter (legacy from ThreadX API)
 * 
 * @details
 * This thread:
 * - Continuously polls the CAN FIFO for incoming messages
 * - Identifies command types (speed vs steering)
 * - Distributes commands via queues to appropriate motor threads
 * - Sets event flags to signal command availability
 * - Runs at priority 2 (highest priority)
 * 
 * Command IDs:
 * - 44 (0x2C): Speed command → queue_speed_cmd
 * - 45 (0x2D): Steering command → queue_steer_cmd
 * 
 * @see queue_speed_cmd, queue_steer_cmd, can_receive()
 */
VOID canRX(ULONG initial_input)
```

### 10.2 Documentation Maintenance

- Run local doxygen generation before commits
- Review warnings: `WARN_IF_UNDOCUMENTED = YES`

### 10.3 CI/CD Integration Benefits

- **Automatic documentation generation** on every push
- **Version history** through GitHub Actions artifacts
- **Backup retention** with 30-60 day retention policies
- **Collaboration** - team members can review current API docs

---

## 11 - Summary Table

| Aspect | Details |
|--------|---------|
| **Language** | C (STM32 + ThreadX RTOS) |
| **Total Threads** | 6 (priorities 2-10) |
| **Communication Queues** | 2 (speed, steering) |
| **Synchronization Primitives** | 1 mutex, 3 event flags |
| **Stack Memory per Thread** | 1KB |
| **Time Slicing** | Disabled (priority-based) |
| **Documentation Trigger** | Automatic on push/PR |
| **Artifact Retention** | 30 days (regular), 60 days (archive) |

---

## 13 - Conclusion

The DrivaPi ThreadX implementation provides a robust embedded real-time system with:
- **6 concurrent threads** managing motor control, sensor input, and CAN communication
- **Prioritized execution** with CAN RX at highest priority for responsive control
- **Thread-safe communication** via queues, event flags, and mutexes
- **Automated documentation** generation via GitHub Actions and Doxygen

The Doxygen workflow ensures that API documentation is continuously generated, validated, and archived, facilitating team collaboration and code maintenance.

---