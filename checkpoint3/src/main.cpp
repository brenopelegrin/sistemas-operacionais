#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

enum MemoryAlgorithm {
  CLOCK,
  LRU
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