#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <regex>

using namespace std;

/**
 * @enum MemoryAlgorithm
 * @brief Algoritmos de substituição de memória.
 */
enum MemoryAlgorithm {
    CLOCK,
    LRU
};

/**
 * @enum ProcessState
 * @brief Estados de processos.
 */
enum ProcessState {
    READY,
    RUNNING,
    SUSPENDED,
    TERMINATED
};

/**
 * @brief Converte um valor do enum ProcessState para sua representação em string.
 * @param state O estado do processo a ser convertido.
 * @return Uma string representando o estado do processo.
 */
string processStateToString(ProcessState state) {
    switch (state) {
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case SUSPENDED: return "SUSPENDED";
        case TERMINATED: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

/**
 * @struct Page
 * @brief Representa uma página na tabela de páginas.
 */
struct Page {
    int processId;
    int pageNumber;
    int frameNumber;        // -1 se não estiver na memória
    bool valid;             // true se a página está na memória física, false caso contrário
    bool dirty;
    bool referenced;        // bit de referência (clock)
    int lastAccessTime;     // timestamp do último acesso (LRU)

    /**
     * @brief Construtor do struct Page.
     * Inicializa uma página como inválida e não pertencente a nenhum processo ou quadro.
     */
    Page() : processId(-1), pageNumber(-1), frameNumber(-1),
             valid(false), dirty(false), referenced(false), lastAccessTime(0) {}
};

/**
 * @struct Frame
 * @brief Representa um quadro de memória física.
 */
struct Frame {
    int frameNumber;
    int processId;          // -1 se o quadro estiver livre
    int pageNumber;         // -1 se o quadro não carrega nenhuma página
    bool occupied;
    int loadTime;           // timestamp de quando a página foi carregada no quadro (LRU)
    bool referenceBit;      // bit de referência (clock)

    /**
     * @brief Construtor do struct Frame.
     * @param num O número identificador do quadro.
     * Inicializa o quadro como não ocupado.
     */
    Frame(int num) : frameNumber(num), processId(-1), pageNumber(-1),
                     occupied(false), loadTime(0), referenceBit(false) {}
};

/**
 * @class PageTable
 * @brief Representa a tabela de páginas de um processo.
 */
class PageTable {
private:
    vector<Page> pages;
    int processId;      // processo que a tabela pertence

public:
    /**
     * @brief Construtor da PageTable.
     * @param pid ID do processo.
     * @param totalPages Número total de páginas virtuais para este processo.
     */
    PageTable(int pid, int totalPages) : processId(pid) {
        pages.resize(totalPages);
        for (int i = 0; i < totalPages; i++) {
            pages[i].processId = pid;
            pages[i].pageNumber = i;
        }
    }

    /**
     * @brief Verifica se uma página específica está carregada na memória física.
     * @param pageNumber O número da página virtual a ser verificada.
     * @return True se a página está carregada (válida) e dentro dos limites, false caso contrário.
     */
    bool isPageLoaded(int pageNumber) {
        if (pageNumber < 0 || pageNumber >= static_cast<int>(pages.size())) return false;
        return pages[pageNumber].valid;
    }

    /**
     * @brief Pega o número do quadro onde uma página virtual está carregada.
     * @param pageNumber O número da página virtual.
     * @return O número do quadro se a página estiver carregada, válida e dentro dos limites, -1 caso contrário.
     */
    int getFrameNumber(int pageNumber) {
        if (pageNumber < 0 || pageNumber >= static_cast<int>(pages.size()) || !pages[pageNumber].valid) return -1;
        return pages[pageNumber].frameNumber;
    }

    /**
     * @brief Mapeia uma página virtual para um quadro.
     * Marca a página como válida e define o número do seu quadro, se o número da página for válido.
     * @param pageNumber O número da página virtual.
     * @param frameNumber O número do quadro.
     */
    void mapPageToFrame(int pageNumber, int frameNumber) {
        if (pageNumber >= 0 && pageNumber < static_cast<int>(pages.size())) {
            pages[pageNumber].frameNumber = frameNumber;
            pages[pageNumber].valid = true;
        }
    }

    /**
     * @brief Invalida uma página virtual.
     * Marca a página como inválida e remove sua associação ao frame, se o número da página for válido.
     * @param pageNumber O número da página virtual a ser invalidada.
     */
    void invalidatePage(int pageNumber) {
        if (pageNumber >= 0 && pageNumber < static_cast<int>(pages.size())) {
            pages[pageNumber].valid = false;
            pages[pageNumber].frameNumber = -1;
        }
    }

    /**
     * @brief Define o bit de modificação (dirty bit) de uma página.
     * @param pageNumber O número da página virtual.
     * @param dirty O valor para o bit de modificação (true se modificada, false caso contrário).
     */
    void setDirty(int pageNumber, bool dirty) {
        if (pageNumber >= 0 && pageNumber < static_cast<int>(pages.size())) {
            pages[pageNumber].dirty = dirty;
        }
    }

    /**
     * @brief Define o bit de referência e o lastAccess de uma página.
     * @param pageNumber O número da página virtual.
     * @param referenced O valor para o bit de referência (true se for referenciada).
     * @param time O clock atual do sistema (no momento do acesso).
     */
    void setReferenced(int pageNumber, bool referenced, int time) {
        if (pageNumber >= 0 && pageNumber < static_cast<int>(pages.size())) {
            pages[pageNumber].referenced = referenced;
            pages[pageNumber].lastAccessTime = time;
        }
    }

    /**
     * @brief Gera uma referência para o vetor de pages.
     * @return Referência para o vetor de structs Page.
     */
    vector<Page>& getPages() { return pages; }

    /**
     * @brief Gera uma referência constante para uma página específica.
     * Cria a exceção std::out_of_range se o número da página for inválido.
     * @param pageNumber O número da página virtual.
     * @return Referência constante para uma Page.
     * @throws std::out_of_range se pageNumber estiver fora dos limites esperados.
     */
    const Page& getPage(int pageNumber) const {
        if (pageNumber < 0 || pageNumber >= static_cast<int>(pages.size())) {
            throw std::out_of_range("Número de página inválido em getPage");
        }
        return pages[pageNumber];
    }

    /**
     * @brief Printa o conteúdo da tabela de páginas.
     * Mostra o mapeamento de cada página para cada quadro ou se ela está no estado swapped.
     */
    void printTable() {
        cout << "  Processo P" << processId << ":" << endl;
        for (size_t i = 0; i < pages.size(); i++) {
            cout << "    Página " << i << " -> ";
            if (pages[i].valid) {
                cout << "Quadro " << pages[i].frameNumber
                     << " [Valid=" << pages[i].valid
                     << ", Dirty=" << pages[i].dirty
                     << ", Referenced=" << pages[i].referenced 
                     << ", LastAccess=" << pages[i].lastAccessTime << "]" << endl;
            } else {
                cout << "Swapped [Valid=0, Dirty=" << pages[i].dirty << ", Referenced=0]" << endl;
            }
        }
    }
};

/**
 * @struct Process
 * @brief Representar um processo no sistema.
 */
struct Process {
    int processId;
    int imageSize;              // em bytes
    int totalPages;             // número de páginas que o processo possui
    ProcessState state;
    PageTable* pageTable;
    int lastPageAccessed;       // número da última página virtual acessada

    /**
     * @brief Construtor do struct Process.
     * @param pid ID do processo.
     * @param size_bytes Tamanho da imagem do processo em bytes.
     * @param pageSize_bytes Tamanho de cada página em bytes.
     * Calcula o número total de páginas e inicializa a tabela de páginas.
     */
    Process(int pid, int size_bytes, int pageSize_bytes)
        : processId(pid), imageSize(size_bytes), state(READY), lastPageAccessed(-1) {
        totalPages = (imageSize + pageSize_bytes - 1) / pageSize_bytes; // Ceiling division
        pageTable = new PageTable(pid, totalPages);
    }

    /**
     * @brief Destrutor do struct Process.
     * Libera a memória alocada para a tabela de páginas.
     */
    ~Process() {
        delete pageTable;
    }
};

/**
 * @class LRUAlgorithm
 * @brief Implementação do algoritmo de substituição de páginas LRU (Least Recently Used).
 * Seleciona a página que não foi utilizada por mais tempo como vítima para substituição.
 */
class LRUAlgorithm {
public:
    /**
     * @brief Construtor do algoritmo LRU.
     */
    LRUAlgorithm() {}

    /**
     * @brief Seleciona um quadro vítima para substituição usando LRU.
     * O quadro vítima é aquele que teve a página carregada (ou referenciada) há mais tempo.
     * @param frames Vetor de frames.
     * @return O número do quadro vítima selecionado. Retorna -1 se nenhum quadro ocupado for encontrado.
     */
    int selectVictimFrame(const vector<Frame>& frames) {
        int oldestTime = INT_MAX;
        int victimFrame = -1;

        for (const auto& frame : frames) {
            // aqui reutilizamos o loadTime como o lastAccessTime para o algoritmo LRU
            if (frame.occupied && frame.loadTime < oldestTime) { 
                oldestTime = frame.loadTime;
                victimFrame = frame.frameNumber;
            }
        }
        if (victimFrame == -1 && !frames.empty()) {
             for (const auto& frame : frames) { if(frame.occupied) return frame.frameNumber;}
        }
        return victimFrame;
    }

    /**
     * @brief Atualiza o tempo de lastAccess de um quadro.
     * Chamado quando uma página em um quadro é acessada.
     * @param frameNumber O número do quadro que foi acessado.
     * @param frames Referência ao vetor de frames.
     * @param globalCurrentTime Referência ao clock global do MemoryManager, usado para marcar o tempo de acesso.
     */
    void updateAccessTime(int frameNumber, vector<Frame>& frames, int& globalCurrentTime) {
        if (frameNumber >= 0 && frameNumber < static_cast<int>(frames.size())) {
            frames[frameNumber].loadTime = globalCurrentTime;
        }
    }
};

/**
 * @class ClockAlgorithm
 * @brief Implementação do algoritmo de substituição de páginas Clock.
 * Usa um ponteiro circular e um bit de referência para selecionar uma página vítima.
 */
class ClockAlgorithm {
private:
    int clockPointer; // clock, indica o próximo frame a ser inspecionado.

public:
    /**
     * @brief Construtor do algoritmo Clock.
     * Inicializa o clock em 0.
     */
    ClockAlgorithm() : clockPointer(0) {}

    /**
     * @brief Seleciona um quadro vítima para substituição usando o algoritmo Clock.
     * Percorre os quadros em um buffer circular. Se um quadro tem o bit de referência 0, ele é escolhido.
     * Se o bit for 1, ele é resetado para 0 e o ponteiro é incrementado (dá uma segunda chance).
     * @param frames Referência ao vetor de frames. O algoritmo modifica os bits de referência, então pode ter efeitos colaterais.
     * @return O número do quadro vítima selecionado. Retorna -1 se o vetor frames estiver vazio.
     */
    int selectVictimFrame(vector<Frame>& frames) {
        if (frames.empty()) return -1;
        while (true) {
            if (static_cast<int>(frames.size()) == 0) return -1;
            clockPointer = clockPointer % static_cast<int>(frames.size());

            if (frames[clockPointer].occupied) {
                if (!frames[clockPointer].referenceBit) { // se o bit de referência é 0, então é usado como vítima
                    int victim = clockPointer;
                    clockPointer = (clockPointer + 1) % static_cast<int>(frames.size());
                    return victim;
                } else {
                    frames[clockPointer].referenceBit = false; // dá uma segunda chance (reseta o bit)
                }
            }
            clockPointer = (clockPointer + 1) % static_cast<int>(frames.size()); // avança o ptr
        }
    }

    /**
     * @brief Define o bit de referência de um quadro específico.
     * @param frameNumber O número do quadro.
     * @param frames Referência ao vetor de frames.
     * @param value O valor para o bit de referência (true ou false).
     */
    void setReferenceBit(int frameNumber, vector<Frame>& frames, bool value) {
        if (frameNumber >= 0 && frameNumber < static_cast<int>(frames.size())) {
            frames[frameNumber].referenceBit = value;
        }
    }
};

/**
 * @class MemoryManager
 * @brief Gerenciador de Memória.
 * Simula o gerenciamento de memória virtual usando paginação.
 * Lida com a criação de processos, alocação de memória, tratamento de page faults,
 * e a aplicação de algoritmos de substituição. Também gera estatísticas úteis sobre a memória.
 */
class MemoryManager {
private:
    int page_size_bytes;
    int frame_size_bytes;
    int logic_address_size_bits;
    int num_frames_in_physical_memory;
    int max_virtual_memory_size_bytes;
    MemoryAlgorithm memory_algorithm;

    vector<Frame> frames;                       // quadros da memória física.
    unordered_map<int, Process*> processes;     // mapa de processos (PID -> Process).
    queue<int> freeFrames;                      // fila de quadros livres

    // Estatísticas
    int pageFaultCount;
    int swapInCount;
    int swapOutCount;
    int totalMemoryAccesses;                    // contador de operações R/W
    int currentTime;                            // clock do sistema
    int current_total_virtual_memory_allocated_bytes;

    // Algoritmos de substituição
    LRUAlgorithm* lruAlgorithm;
    ClockAlgorithm* clockAlgorithm;

    /**
     * @brief Aloca e inicializa os quadros da memória física.
     * Inicializa os frames e a fila freeFrames.
     */
    void _allocate_physical_memory() {
        frames.clear();
        for (int i = 0; i < num_frames_in_physical_memory; i++) {
            frames.emplace_back(i);
            freeFrames.push(i);
        }
        cout << "[MemoryManager] Alocados " << this->num_frames_in_physical_memory
             << " quadros de " << this->frame_size_bytes << " bytes cada na memória física." << endl;
        cout << "[MemoryManager] Memória física total: " << this->num_frames_in_physical_memory * this->frame_size_bytes << " bytes." << endl;
    }

    /**
     * @brief Verifica se uma string contém apenas caracteres binários ('0' ou '1').
     * @param str A string a ser validada.
     * @return True se a string é um número binário válido e não vazio, false caso contrário.
     */
    bool isValidBinary(const string& str) {
        return !str.empty() &&
               str.find_first_not_of("01") == string::npos;
    }

    /**
     * @brief Verifica se uma string contém apenas dígitos decimais ('0'-'9').
     * @param str A string a ser validada.
     * @return True se a string é um número decimal válido e não vazio, false caso contrário.
     */
    bool isValidDecimal(const string& str) {
        return !str.empty() &&
               str.find_first_not_of("0123456789") == string::npos;
    }

    /**
     * @brief Verifica se um operando de endereço está no formato Tipo A "(x)2".
     * @param operand A string do operando.
     * @return True se o formato corresponde ao Tipo A, false caso contrário.
     */
    bool isTypeAFormat(const string& operand) {
        return operand.length() >= 4 &&
               operand[0] == '(' &&
               operand[operand.length()-1] == '2' &&
               operand[operand.length()-2] == ')';
    }

    /**
     * @brief Converte um operando de endereço tipo A, ou seja, no formato "(x)2" para um inteiro.
     * O valor 'x' é extraído e convertido para inteiro.
     * @param operand A string do operando no formato Tipo A.
     * @return O valor decimal de 'x'.
     */
    int convertTypeAAddress(const string& operand) {
        string numStr = operand.substr(1, operand.length() - 3);
        return stoi(numStr);
    }

    /**
     * @brief Converte um operando de endereço do Tipo B, ou seja, no formato binário, para um inteiro.
     * @param operand A string representando o número binário no formato Tipo B.
     * @return O valor decimal correspondente ao binário.
     */
    int convertTypeBAddress(const string& operand) {
        int result = 0;
        for (char c : operand) {
            result = result * 2 + (c - '0');
        }
        return result;
    }

    /**
     * @brief Traduz um endereço virtual em número da página e deslocamento (offset).
     * @param virtualAddress O endereço virtual a ser traduzido.
     * @return Um par (pair) contendo o número da página e o deslocamento.
     */
    pair<int, int> translateVirtualAddress(int virtualAddress) {
        int pageNumber = virtualAddress / page_size_bytes;
        int offset = virtualAddress % page_size_bytes;
        return make_pair(pageNumber, offset);
    }

    /**
     * @brief Trata uma page fault.
     * Incrementa o contador de page faults, avança o tempo do sistema. Se tiver quadros livres, aloca um.
     * Caso contrário, seleciona um quadro vítima usando o algoritmo configurado,
     * então faz swap out da página antiga (se necessário) e swap in da nova página.
     * Atualiza a tabela de páginas do processo e o estado do quadro.
     * @param processId ID do processo que causou a falta de página.
     * @param pageNumber Número da página virtual que faltou.
     */
    void handlePageFault(int processId, int pageNumber) {
        pageFaultCount++;
        currentTime++; 
        cout << "    [PAGE FAULT] Processo P" << processId << ", Página " << pageNumber << " (Tempo: " << currentTime << ")" << endl;

        int frameNumber;
        Process* process = processes[processId];

        if (process->pageTable->isPageLoaded(pageNumber)) {
            cout << "    [WARNING] Página " << pageNumber << " já está carregada! (handlePageFault check)" << endl;
            return;
        }

        if (!freeFrames.empty()) {
            frameNumber = freeFrames.front();
            freeFrames.pop();
        } else {
            cout << "    [SUBSTITUIÇÃO] Memória cheia, selecionando quadro vítima." << endl;
            if (memory_algorithm == LRU) {
                frameNumber = lruAlgorithm->selectVictimFrame(frames);
            } else { 
                frameNumber = clockAlgorithm->selectVictimFrame(frames);
            }
            if (frameNumber == -1) {
                 cerr << "[ERROR CRÍTICO] Nenhum quadro vítima pôde ser selecionado!" << endl;
                 return;
            }
            swapOut(frameNumber);
        }

        swapIn(processId, pageNumber, frameNumber);
        process->pageTable->mapPageToFrame(pageNumber, frameNumber);
        process->pageTable->setReferenced(pageNumber, true, currentTime); 
        process->pageTable->setDirty(pageNumber, false); // páginas recém carregadas devem estar limpas (dirty=false)

        frames[frameNumber].occupied = true;
        frames[frameNumber].processId = processId;
        frames[frameNumber].pageNumber = pageNumber;
        frames[frameNumber].loadTime = currentTime; // loadTime (LRU) ou lastAccess (Clock)
        frames[frameNumber].referenceBit = true;    // bit de referência (Clock)
    }

    /**
     * @brief Realiza a operação de swap out de uma página de um quadro.
     * Incrementa o contador de swap out. A página no quadro especificado é removida da memória física
     * (marcada como inválida na tabela de páginas do processo). Se a página estiver suja (dirty),
     * escreve ela na memória secundária (geramos apenas um log para simplificar).
     * O quadro físico é então marcado como livre.
     * @param frameNumber O número do quadro a ser liberado.
     */
    void swapOut(int frameNumber) {
        swapOutCount++;
        Frame& frame = frames[frameNumber];
        if (frame.occupied) {
            Process* process = processes[frame.processId];
            const Page& page_to_swap_out = process->pageTable->getPage(frame.pageNumber);

            cout << "    [SWAP OUT] Processo P" << frame.processId
                 << ", Página " << frame.pageNumber
                 << " (Dirty=" << page_to_swap_out.dirty << ")"
                 << " removida do Quadro " << frameNumber << " (Tempo: " << currentTime << ")" << endl;
            
            process->pageTable->invalidatePage(frame.pageNumber);

            frame.occupied = false;
            frame.processId = -1;
            frame.pageNumber = -1;
            frame.referenceBit = false; 
            frame.loadTime = 0; 
        }
    }

    /**
     * @brief Realiza a operação de swap in de uma página para um quadro.
     * Incrementa o contador de swap in.
     * @param processId ID do processo ao qual a página pertence.
     * @param pageNumber Número da página virtual a ser carregada.
     * @param frameNumber Número do quadro onde a página será carregada.
     */
    void swapIn(int processId, int pageNumber, int frameNumber) {
        swapInCount++;
        cout << "    [SWAP IN] Processo P" << processId
             << ", Página " << pageNumber
             << " carregada no Quadro " << frameNumber << " (Tempo: " << currentTime << ")" << endl;
    }

public:
    /**
     * @brief Construtor do MemoryManager.
     * @param page_size_bytes_param Tamanho de cada página em bytes.
     * @param frame_size_bytes_param Tamanho de cada quadro em bytes.
     * @param logic_address_size_bits_param Número de bits do endereço lógico.
     * @param num_frames_in_physical_memory_param Número total de quadros na memória física.
     * @param max_virtual_memory_size_bytes_param Limite máximo de memória virtual total do sistema.
     * @param memory_algorithm_param Algoritmo de substituição de página a ser usado (LRU ou Clock).
     */
    MemoryManager(
        int page_size_bytes_param,
        int frame_size_bytes_param,
        int logic_address_size_bits_param,
        int num_frames_in_physical_memory_param,
        int max_virtual_memory_size_bytes_param,
        MemoryAlgorithm memory_algorithm_param) :
        page_size_bytes(page_size_bytes_param),
        frame_size_bytes(frame_size_bytes_param),
        logic_address_size_bits(logic_address_size_bits_param),
        num_frames_in_physical_memory(num_frames_in_physical_memory_param),
        max_virtual_memory_size_bytes(max_virtual_memory_size_bytes_param),
        memory_algorithm(memory_algorithm_param),
        pageFaultCount(0),
        swapInCount(0),
        swapOutCount(0),
        totalMemoryAccesses(0),
        currentTime(0),
        current_total_virtual_memory_allocated_bytes(0)
    {
        lruAlgorithm = new LRUAlgorithm();
        clockAlgorithm = new ClockAlgorithm();
        this->_allocate_physical_memory();
        cout << "[MemoryManager] Limite máximo de memória virtual configurado: " << this->max_virtual_memory_size_bytes << " bytes." << endl;
    }

    /**
     * @brief Destrutor do MemoryManager.
     * Libera a memória alocada para os processos e os algoritmos de substituição.
     */
    ~MemoryManager() {
        for (auto& pair : processes) {
            delete pair.second;
        }
        delete lruAlgorithm;
        delete clockAlgorithm;
    }

    /**
     * @brief Cria um novo processo no sistema.
     * Verifica se o processo já existe e se há espaço na memória virtual total do sistema.
     * Aloca um novo Process e o adiciona ao mapa de processos.
     * @param pid ID do novo processo.
     * @param image_size_bytes Tamanho da imagem do novo processo em bytes.
     */
    void createProcess(int pid, int image_size_bytes) {
        if (processes.find(pid) != processes.end()) {
            cout << "[ERROR] Processo P" << pid << " já existe!" << endl;
            return;
        }

        if (current_total_virtual_memory_allocated_bytes + image_size_bytes > max_virtual_memory_size_bytes) {
            cout << "[ERROR] Falha ao criar Processo P" << pid << " (tamanho: " << image_size_bytes << " bytes)." << endl;
            cout << "    Memória virtual total solicitada (" << current_total_virtual_memory_allocated_bytes + image_size_bytes << " bytes)"
                 << " excederia o limite máximo do sistema (" << max_virtual_memory_size_bytes << " bytes)." << endl;
            return;
        }

        Process* newProcess = new Process(pid, image_size_bytes, page_size_bytes);
        processes[pid] = newProcess;
        current_total_virtual_memory_allocated_bytes += image_size_bytes;

        cout << "[MemoryManager] Processo P" << pid << " (tamanho: " << image_size_bytes << " bytes) criado com "
             << newProcess->totalPages << " páginas. Estado: " << processStateToString(newProcess->state) << "." << endl;
        cout << "    Memória virtual total alocada no sistema: " << current_total_virtual_memory_allocated_bytes << " bytes." << endl;
    }

    /**
     * @brief Simula um acesso à memória (R/W) por um processo.
     * Traduz o endereço virtual, trata page faults se necessário, e atualiza
     * os bits de referência/dirty e metadados dos algoritmos de substituição.
     * @param pid ID do processo que está acessando a memória.
     * @param type Tipo de acesso (R ou W).
     * @param addr Endereço virtual a ser acessado.
     */
    void accessMemory(int pid, char type, int addr) {
        totalMemoryAccesses++;
        currentTime++; 

        if (processes.find(pid) == processes.end()) {
            cout << "[ERROR] Processo P" << pid << " não existe! (Tempo: " << currentTime << ")" << endl;
            return;
        }

        Process* process = processes[pid];
        ProcessState originalState = process->state;
        process->state = RUNNING; 

        if (addr < 0 || addr >= process->imageSize ) {
            cout << "[ERROR] Endereço " << addr << " fora dos limites para o Processo P" << pid
                 << " (Tamanho: " << process->imageSize << " bytes, Estado: " << processStateToString(process->state) << ", Tempo: " << currentTime << ")" << endl;
            process->state = originalState; 
            return;
        }

        auto [pageNumber, offset] = translateVirtualAddress(addr);

        cout << "[ACESSO] P" << pid << " (" << processStateToString(process->state) << ") " 
             << type << " End:" << addr << " (Página: " << pageNumber << ", Offset: " << offset << ", Tempo: " << currentTime << ")" << endl;

        bool pageLoaded = process->pageTable->isPageLoaded(pageNumber);

        if (!pageLoaded) {
            handlePageFault(pid, pageNumber);
            pageLoaded = process->pageTable->isPageLoaded(pageNumber);
            if (!pageLoaded) {
                 cout << "[ERROR CRÍTICO] Falha ao carregar página " << pageNumber << " para P" << pid << " após page fault. (Tempo: " << currentTime << ")" << endl;
                 process->state = READY; 
                 return;
            }
        }
        
        int frameNumber = process->pageTable->getFrameNumber(pageNumber);

        if (frameNumber >= 0) { 
            process->pageTable->setReferenced(pageNumber, true, currentTime);

            if (type == 'W') {
                process->pageTable->setDirty(pageNumber, true);
                 cout << "    [MODIFICADO] Página " << pageNumber << " do Processo P" << pid << " marcada como dirty." << endl;
            }

            if (memory_algorithm == LRU) {
                lruAlgorithm->updateAccessTime(frameNumber, frames, this->currentTime);
            } else { 
                clockAlgorithm->setReferenceBit(frameNumber, frames, true);
            }
        } else {
             cout << "[ERROR CRÍTICO] Página " << pageNumber << " do Processo P" << pid << " não encontrada em um quadro após tentativa de carga! (Tempo: " << currentTime << ")" << endl;
        }
        
        process->lastPageAccessed = pageNumber;
        process->state = READY; 
    }

    /**
     * @brief Simula a execução de uma instrução de CPU ou uma operação de E/S por um processo.
     * Atualiza o estado do processo de acordo com o tipo de instrução.
     * @param pid ID do processo executando a instrução.
     * @param type Tipo de instrução (P para CPU, I para E/S).
     * @param operand Operando da instrução.
     */
    void executeInstruction(int pid, char type, int operand) {
        currentTime++; 
        if (processes.find(pid) == processes.end()) {
            cout << "[ERROR] Processo P" << pid << " não existe! (Tempo: " << currentTime << ")" << endl;
            return;
        }
        
        Process* process = processes[pid];
        process->state = RUNNING; 

        if (type == 'P') {
            cout << "[CPU] Processo P" << pid << " (" << processStateToString(process->state) 
                 << ") executando instrução (operando: " << operand << ", Tempo: " << currentTime << ")" << endl;
            process->state = READY; 
        } else if (type == 'I') {
            cout << "[I/O] Processo P" << pid << " (" << processStateToString(process->state) 
                 << ") executando E/S (dispositivo: " << operand << ", Tempo: " << currentTime << ")" << endl;
            process->state = SUSPENDED; 
        }
    }

    /**
     * @brief Imprime o estado atual da memória física e secundária, e o estado dos processos.
     * Mostra quais páginas estão em quais quadros, quais estão em swap, e o estado de cada processo.
     */
    void printMemoryState() {
        cout << "\n========== ESTADO DA MEMÓRIA (Tempo: " << currentTime << ") ==========" << endl;
        cout << "Memória Física (" << num_frames_in_physical_memory << " quadros de " << frame_size_bytes << " bytes):" << endl;
        for (int i = 0; i < num_frames_in_physical_memory; i++) {
            cout << "  Quadro " << i << ": ";
            if (frames[i].occupied) {
                cout << "Processo P" << frames[i].processId
                     << ", Página " << frames[i].pageNumber;
                if (memory_algorithm == CLOCK) {
                    cout << " (Referenced=" << frames[i].referenceBit << ")";
                } else { 
                    cout << " (LoadTime=" << frames[i].loadTime << ")";
                }
            } else {
                cout << "Livre";
            }
            cout << endl;
        }

        cout << "\nMemória Secundária (Páginas não carregadas):" << endl;
        bool anySwapped = false;
        for (auto& pair : processes) {
            Process* process = pair.second;
            vector<int> swappedPagesNumbers; 

            for (const auto& page : process->pageTable->getPages()) {
                if (!page.valid) {
                    swappedPagesNumbers.push_back(page.pageNumber);
                }
            }
            
            if (!swappedPagesNumbers.empty()) {
                anySwapped = true;
                cout << "  Processo P" << process->processId << ": Páginas [";
                for (size_t i = 0; i < swappedPagesNumbers.size(); i++) {
                    int pageNum = swappedPagesNumbers[i];
                    const Page& p_struct = process->pageTable->getPage(pageNum); 
                    cout << pageNum << "(Dirty=" << p_struct.dirty << ")";
                    if (i < swappedPagesNumbers.size() - 1) cout << ", ";
                }
                cout << "]" << endl;
            }
        }
        if (!anySwapped) {
            cout << "  Nenhuma página em memória secundária." << endl;
        }

        cout << "\nEstado dos Processos:" << endl;
        if (processes.empty()) {
            cout << "  Nenhum processo no sistema." << endl;
        } else {
            for (auto& pair : processes) {
                Process* process = pair.second;
                cout << "  Processo P" << process->processId 
                     << ": " << processStateToString(process->state)
                     << " (Páginas: " << process->totalPages 
                     << ", Tamanho: " << process->imageSize << " bytes)" << endl;
            }
        }
        cout << "Memória Virtual Total Alocada: " << current_total_virtual_memory_allocated_bytes << " / " << max_virtual_memory_size_bytes << " bytes." << endl;

        cout << "\n================= FIM ESTADO MEMÓRIA =================\n" << endl;
    }

    /**
     * @brief Imprime as tabelas de páginas de todos os processos.
     */
    void printPageTables() {
        cout << "\n========== TABELAS DE PÁGINAS (Tempo: " << currentTime << ") ==========" << endl;
        for (auto& pair : processes) {
            pair.second->pageTable->printTable();
        }
        cout << "\n================= FIM TABELAS ==================\n" << endl;
    }

    /**
     * @brief Imprime as estatísticas da simulação.
     * Inclui número de page faults, swaps, acessos à memória, taxa de hit e tempo total simulado.
     */
    void printStatistics() {
        cout << "\n========== ESTATÍSTICAS (Final - Tempo: " << currentTime << ") ==========" << endl;
        cout << "Page Faults: " << pageFaultCount << endl;
        cout << "Operações de Swap In: " << swapInCount << endl;
        cout << "Operações de Swap Out: " << swapOutCount << endl;
        cout << "Total de Acessos à Memória (operações R/W): " << totalMemoryAccesses << endl;

        if (totalMemoryAccesses > 0) {
            double hitRate = ((double)(totalMemoryAccesses - pageFaultCount) / totalMemoryAccesses) * 100.0;
            cout << "Taxa de Hit (para R/W): " << fixed << setprecision(1) << hitRate << "%" << endl;
        }

        cout << "Algoritmo usado: " << (memory_algorithm == LRU ? "LRU" : "Clock") << endl;
        cout << "Total de tempo lógico simulado: " << currentTime << " unidades." << endl;
        cout << "\n============== FIM ESTATÍSTICAS ===============\n" << endl;
    }

    /**
     * @brief Carrega e processa um arquivo de entrada contendo comandos para o simulador.
     * Lê cada linha do arquivo, parseia o comando e o executa através dos métodos apropriados do MemoryManager.
     * @param filename O nome do arquivo de entrada.
     */
    void loadInputFile(const string& filename) {
        ifstream infile(filename);
        if (!infile.is_open()) {
            cerr << "[ERROR] Não foi possível abrir o arquivo: " << filename << endl;
            return;
        }

        string line;
        int lineNumber = 0;
        cout << "\n========== PROCESSANDO ARQUIVO (" << filename << ") ==========" << endl;

        while (getline(infile, line)) {
            lineNumber++;

            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            size_t source_pos = line.find("] ");
            if (line[0] == '[' && source_pos != string::npos) {
                line = line.substr(source_pos + 2);
            }

            stringstream ss(line);
            string processName, operation, operand;

            if (!(ss >> processName >> operation >> operand)) {
                cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Formato inválido (esperado: PID OP OPERANDO)" << endl;
                continue;
            }

            if (operation.length() != 1) {
                cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Operação deve ter 1 caractere" << endl;
                continue;
            }

            if (processName[0] != 'P' || !isValidDecimal(processName.substr(1))) {
                cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Nome de processo inválido (esperado: P<num>)" << endl;
                continue;
            }

            int pid = stoi(processName.substr(1));
            char op = operation[0];

            cout << "\n[COMANDO " << lineNumber << "] " << processName << " " << op << " " << operand << endl;
            
            if (processes.count(pid) && processes[pid]->state == SUSPENDED && (op == 'R' || op == 'W' || op == 'P')) {
                cout << "    [AVISO] Processo P" << pid << " está SUSPENSO mas recebeu comando '" << op << "'. O comando será processado." << endl;
            }

            switch (op) {
                case 'C': { // Criação de processo
                    if (!isValidDecimal(operand)) {
                        cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Tamanho deve ser número decimal para operação 'C'" << endl;
                        continue;
                    }
                    int size = stoi(operand);
                    if (size <= 0) {
                        cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Tamanho deve ser positivo para operação 'C'" << endl;
                        continue;
                    }
                    this->createProcess(pid, size);
                    printMemoryState(); 
                    break;
                }

                case 'R': // Operação de leitura
                case 'W': { // Operação de escrita
                    int addr;
                    if (isTypeAFormat(operand)) { // Endereço tipo decimal (x)2
                        string numPart = operand.substr(1, operand.length() - 3);
                        if (!isValidDecimal(numPart)) {
                            cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Número inválido no formato (x)2 para operação '" << op << "'" << endl;
                            continue;
                        }
                        addr = convertTypeAAddress(operand);
                    } else { // Endereço tipo binário
                        if (!isValidBinary(operand)) {
                            cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Endereço binário inválido para operação '" << op << "'" << endl;
                            continue;
                        }
                        addr = convertTypeBAddress(operand);
                    }
                    this->accessMemory(pid, op, addr);
                    printMemoryState(); 
                    break;
                }

                case 'P': // Instrução de CPU
                case 'I': { // Operação de E/S
                    int operandValue;
                    if (isTypeAFormat(operand)) { // Operando tipo decimal (x)2
                        string numPart = operand.substr(1, operand.length() - 3);
                        if (!isValidDecimal(numPart)) {
                            cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Número inválido no formato (x)2 para operação '" << op << "'" << endl;
                            continue;
                        }
                        operandValue = convertTypeAAddress(operand);
                    } else { // Operando tipo binário
                        if (!isValidBinary(operand)) {
                            cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Operando binário inválido para operação '" << op << "'" << endl;
                            continue;
                        }
                        operandValue = convertTypeBAddress(operand);
                    }
                    this->executeInstruction(pid, op, operandValue);
                    printMemoryState(); 
                    break;
                }

                default:
                    cout << "ERRO na linha " << lineNumber << " (\"" << line << "\"): Operação '" << op
                         << "' não reconhecida" << endl;
                    continue;
            }
        }

        infile.close();
        printPageTables();
        printStatistics();
        cout << "\n=============== FIM ARQUIVO ==============\n" << endl;
    }
};

// Função para remover espaços em branco de uma string (início e fim)
std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return ""; // String vazia ou apenas espaços em branco
    }
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// Fnção para mostrar a mensagem de help
void print_help(const char* program_name) {
    std::cout << "\nUso: " << program_name << " [opções] <arquivo_de_entrada>\n\n";
    std::cout << "Simulador de Memória Virtual.\n\n";
    std::cout << "Argumentos posicionais obrigatórios:\n";
    std::cout << "  <arquivo_de_entrada>            Caminho para o arquivo contendo as instruções de simulação.\n\n";
    std::cout << "Opções (argumentos de palavra-chave):\n";
    std::cout << "  --page_size <bytes>             Tamanho de cada página em bytes (ex: 4096). Padrão: 4096.\n";
    std::cout << "  --frame_size <bytes>            Tamanho de cada quadro em bytes. Padrão: igual ao page_size.\n";
    std::cout << "  --logic_address_size <bits>     Número de bits no endereço lógico (ex: 16 bits). Padrão: 16.\n";
    std::cout << "  --num_frames <num>              Número total de quadros na memória física (ex: 8). Padrão: 8.\n";
    std::cout << "  -h, --help                      Mostra esta mensagem de ajuda e sai.\n\n";
    std::cout << "Exemplo:\n";
    std::cout << "  " << program_name << " --page_size 8192 --num_frames 4 entrada.txt\n";
}

/**
 * @brief Função principal do simulador de memória virtual.
 * Configura os parâmetros iniciais do sistema, instancia o MemoryManager,
 * e carrega o arquivo de entrada para iniciar a simulação.
 * @param argc Número de args da linha de comando.
 * @param argv Vetor de strings dos args.
 * @return 0 se a execução for bem-sucedida, um código de erro caso contrário.
 */
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            print_help(argv[0]);
            return 0;
        }
    }

    int pageSize = 4096;        
    int frameSize = pageSize; 
    int logicAddressBits = 16; 
    int numFrames = 8;          
    int maxVirtualMemorySystem = pageSize * (numFrames * 2); 
    MemoryAlgorithm algorithm = LRU;
    std::string input_filename = "data/input.txt";

    std::vector<std::string> positional_args;
    std::unordered_map<std::string, std::string> keyword_args_map;

    // Faz o parsing dos argumentos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--", 0) == 0) {
            std::string key = arg.substr(2);
            if (i + 1 < argc) {
                std::string next_arg = argv[i + 1];
                if (next_arg.rfind("--", 0) != 0 && next_arg.rfind("-",0) !=0) {
                    keyword_args_map[key] = trim(next_arg);
                    i++;
                } else {
                    std::cerr << "[AVISO] Argumento '" << arg << "' fornecido sem valor. Ignorando." << std::endl;
                }
            } else {
                std::cerr << "[AVISO] Argumento '" << arg << "' fornecido sem valor. Ignorando." << std::endl;
            }
        } else {
            positional_args.push_back(trim(arg));
        }
    }

    if (positional_args.size() == 1) {
        input_filename = positional_args[0];
    } else if (positional_args.empty()) {
        std::cerr << "[ERRO] Arquivo de entrada não fornecido.\n";
        print_help(argv[0]);
        return 1;
    } else {
        std::cerr << "[ERRO] Múltiplos argumentos posicionais fornecidos. Esperado apenas um: <arquivo_de_entrada>.\n";
        print_help(argv[0]);
        return 1;
    }
    
    try {
        if (keyword_args_map.count("page_size")) {
            pageSize = std::stoi(keyword_args_map["page_size"]);
        }
        if (keyword_args_map.count("frame_size")) {
            frameSize = std::stoi(keyword_args_map["frame_size"]);
        } else if (keyword_args_map.count("page_size")) { 
            frameSize = pageSize;
        }
        if (keyword_args_map.count("logic_address_size")) {
            logicAddressBits = std::stoi(keyword_args_map["logic_address_size"]);
        }
        if (keyword_args_map.count("num_frames")) {
            numFrames = std::stoi(keyword_args_map["num_frames"]);
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "[ERRO] Argumento inválido para conversão numérica fornecido para uma das opções: " << e.what() << std::endl;
        print_help(argv[0]);
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "[ERRO] Valor numérico fora do intervalo fornecido para uma das opções: " << e.what() << std::endl;
        print_help(argv[0]);
        return 1;
    }
    
    if (pageSize <= 0 || frameSize <= 0 || logicAddressBits <= 0 || numFrames <= 0) {
        std::cerr << "[ERRO] page_size, frame_size, logic_address_size e num_frames devem ser valores positivos." << std::endl;
        print_help(argv[0]);
        return 1;
    }

    std::cout << "========== SIMULADOR DE MEMÓRIA VIRTUAL ==========" << std::endl;
    std::cout << "\nConfigurações do sistema:" << std::endl;
    std::cout << "  Arquivo de entrada: " << input_filename << std::endl;
    std::cout << "  Tamanho da página: " << pageSize << " bytes (" << pageSize / 1024.0 << " KB)" << std::endl;
    std::cout << "  Tamanho do quadro: " << frameSize << " bytes (" << frameSize / 1024.0 << " KB)" << std::endl;
    std::cout << "  Número de quadros na memória física: " << numFrames << std::endl;
    std::cout << "  Tamanho total da memória física: " << numFrames * frameSize / 1024.0 << " KB" << std::endl;
    std::cout << "  Algoritmo de substituição: " << (algorithm == LRU ? "LRU" : "Clock") << std::endl;
    std::cout << "  Tamanho dos endereços lógicos: " << logicAddressBits << " bits" << std::endl;
    std::cout << "  Limite máximo de memória virtual (calculado): " << maxVirtualMemorySystem / 1024.0 << " KB (" << maxVirtualMemorySystem << " bytes)" << std::endl;
    std::cout << "\n=================================================\n" << std::endl;

    MemoryManager mm(pageSize, frameSize, logicAddressBits, numFrames, maxVirtualMemorySystem, algorithm);
    
    mm.loadInputFile(input_filename);

    return 0;
}