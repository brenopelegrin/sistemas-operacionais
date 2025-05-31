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

class MemoryManager {
  private:
    int page_size_bytes;
    int frame_size_bytes;
    int logic_address_size_bits;
    int num_frames_in_physical_memory;
    int max_virtual_memory_size;
    MemoryAlgorithm memory_algorithm;
    
    std::vector<void*> frames;

    void _free_physical_memory(){
      // Free the allocated frames
      for(int i=this->frames.size(); i>0; i--){
        void* p = this->frames.back();
        free(p);
        this->frames.pop_back();
      }
    }

    void _allocate_physical_memory(){
      for(int i=0; i<num_frames_in_physical_memory; i++){
        // Keep trying to allocate memory if it isn't available
        void* p = NULL;
        while(p == NULL){
          p = malloc(frame_size_bytes);
        }
        // Put the pointer into the frames vector after getting it from malloc
        this->frames.push_back(p);
      }
      cout << "[MemoryManager] " << "Allocated " << this->num_frames_in_physical_memory << " frames of " << this->frame_size_bytes << " bytes each in physical memory" << endl;
    }
  public:
    MemoryManager(
      int page_size_bytes, 
      int frame_size_bytes, 
      int logic_address_size_bits,
      int num_frames_in_physical_memory, 
      int max_virtual_memory_size,
      MemoryAlgorithm memory_algorithm) {
        this->page_size_bytes = page_size_bytes;
        this->frame_size_bytes = frame_size_bytes;
        this->logic_address_size_bits = logic_address_size_bits;
        this->num_frames_in_physical_memory = num_frames_in_physical_memory;
        this->max_virtual_memory_size = max_virtual_memory_size;
        this->memory_algorithm = memory_algorithm;

        this->_allocate_physical_memory();
    };
    ~MemoryManager(){
      this->_free_physical_memory();
    };

    void createProcess(int pid, int size) {
      return;
    };
    void accessMemory(int pid, int type, int addr){
      return;
    };
    void executeInstruction(int pid, int type, int operand){
      return;
    };

    void loadInputFile(const string& filename) {
      ifstream infile(filename);
      string line;
      while (getline(infile, line)) {
          istringstream iss(line);
          string pidStr;
          char type;
          string arg1;

          iss >> pidStr >> type >> arg1;
          int pid = stoi(pidStr.substr(1));

          if (type == 'C') {
              int size = stoi(arg1);
              this->createProcess(pid, size);
              cout << "[MemoryManager] " << "Creating process with pid=" << pid <<", size=" << size << "." << endl;
          } 
          else if (type == 'R' || type == 'W') {
              if (arg1[0] == 'b') {
                  int addr = stoi(arg1.substr(1), nullptr, 2);
                  this->accessMemory(pid, type, addr);
                  cout << "[MemoryManager] " << "Accessing memory with pid=" << pid << ", type=" << type << ", addr=" << addr << endl;
              } else {
                  cerr << "[MemoryManager] [ERROR] Invalid address: " << arg1 << endl;
              }
          } 
          else if (type == 'P' || type == 'I') {
              if (arg1[0] == 'b') {
                  int operand = stoi(arg1.substr(1), nullptr, 2);
                  this->executeInstruction(pid, type, operand);
                  cout << "[MemoryManager] " << "Executing instruction with pid=" << pid << ", type=" << type << ", operand=" << operand << endl;
              } else {
                  cerr << "[MemoryManager] [ERROR] Invalid operand: " << arg1 << endl;
              }
          }
      }
    }
};

int main(int argc, char* argv[]){
  std::cout << "Hello World!" << std::endl;
  enum MemoryAlgorithm algorithm = CLOCK;
  MemoryManager mm(1, 100, 1, 50, 5, algorithm);
  std::string filename = "data/input.txt";
  mm.loadInputFile(filename);
  return 0;
}