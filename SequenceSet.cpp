#include <SD.h>
#include <Arduino.h>
#include "Bloco.cpp"

class SequenceSet{
  public:
    SequenceSet(int ord){
      buff = NULL;
      primeiro = -1;
      ultimo = -1;
      tamanho = 0;
      ordem = ord;
      gravarCabecalho();
    }
    ~SequenceSet(){
		//SD.remove("SS.ss");
		delete buff;
	}
	void inserir(int valor){
		//Se o tamanho for 0, a estrutura estará vazia, logo precisa-se
		//instanciar um novo Bloco e associá-lo a primeira e ultima
		//posição, além de apontar o buff para a mesma
		if(buff == 0){
			buff = new Bloco(ordem);
			buff->setEndereco(0);
			buff->inserir(valor);
			tamanho++;
			primeiro = 0;
			ultimo = 0;
		}
		//Nesse caso, o buff está marcado como disponível para inserção
		//logo, somente precisamos inserir o valor em seu Bloco e nada
		//mais
		else if(buff->getFlag() == '1'){
			buff->inserir(valor);
		}
		//Nesse caso, o buff atual não pode ser utilizado, por isso ele
		//deve ser atualizado
		else{
			//Na lógica utilizada, para fins de otimização das operações
			//com arquivo, utiliza-se um buff em RAM, salvado-o apenas
			//quando necessário
			gravarBuff();
			//Pega-se a próxima posição válida para inserção no arquivo
			int pos = enderecoValido();
			//Salva-se a antiga posição do buff para gravar como
			//anterior ao novo buff
			int antigaPos = buff->getEndereco();
			//O buff passa a apontar para a próxima posição válida
			getBlocoFromId(pos, *buff);
			//No caso da flag do novo buff ser '1', ele é um bloco com 
			//espaços válidos, logo não muda sua posição na estrutura
			if(buff->getFlag() != '1'){
				//A proxima posição válida, é também a próxima posição do
				//Bloco atual
				buff->setProximo(pos);
				//A antiga posição do buff é também a anterior do Bloco
				//atual
				buff->setAnterior(antigaPos);
				//O bloco reutilizado ou recém criado, na lógica usada,
				//passa a ser o 'ultimo' da estrutura
				ultimo = pos;
				//E a estrutura ganha um novo Bloco utilizável
				tamanho++;
			}
			//Com todo o cabeçalho arrumado resta inserir o valor no buff
			buff->inserir(valor);
		}
		//Lembrando que nessa estutura existe um buff em RAM, por isso
		//mesmo com uma nova instância, essa não será apagada em momento
		//algum (fora no destrutor)
	}
	void remover(int valor){
		if(tamanho != 0){
			int pos = buscar(valor);
			//O valor -1 está reservado para NULL, ou nesse caso, 
			//inexistente
			if(pos != -1){
				Bloco *temp = new Bloco(ordem);
				getBlocoFromId(pos, *temp);
				temp->remover(valor);
				//Nesse caso, o bloco ficou vazio, e deve ser retirado 
				//da sequencia da estrutura
				if(temp->getFlag() == '3'){
					tamanho--;
					//Salva os valores de anterior e proximo do bloco
					//excluído para arrumar seus valores
					int anterior = temp->getAnterior();
					int proximo = temp->getProximo();
					//Arruma o anterior e proximo do bloco excluído
					getBlocoFromId(anterior, *temp);
					temp->setProximo(proximo);
					getBlocoFromId(proximo, *temp);
					temp->setAnterior(anterior);
					delete temp;
				}
			}
		}
	}
	int buscar(int valor){
		Bloco *temp = new Bloco(ordem);
		int aux = primeiro;
		int pos;
		while(aux != -1){
			getBlocoFromId(aux, *temp);
			pos = temp->buscar(valor);
			if(pos == -1){
				aux = temp->getProximo();
			}
			else
				return aux;
		}
		delete temp;
	}
  void mergeSort(){
    Bloco* temp = new Bloco(ordem);
    int aux = primeiro;
    int menor;
    int remover;
    int j = 0;
    while(j < ordem){
      getBlocoFromIdAux(primeiro, *temp);
      menor = temp->getValor(0);
      for(int i = 0; i < tamanho; i++){
        remover = temp->getEndereco();
        if((menor < temp->getValor(0) and temp->getValor(0) != -1) or menor == -1){
          menor = temp->getValor(0);
          remover = temp->getEndereco();
        }
      }
      getBlocoFromIdAux(remover, *temp);
      gravarTxt(temp->removerPos(0));
      if(menor == -1)
        j = ordem;
      j++;
    }
    delete temp;
  }
  private:
	  Bloco* buff;
    int primeiro;
    int ultimo;
    int tamanho;
    int ordem;
    
    void gravarCabecalho(){
  		File gravar;
      gravar = SD.open("SS.ss", FILE_WRITE);
      if(gravar){
        gravar.write((const char *) (this), sizeof(SequenceSet));
      }
      gravar.close();
  	}
  	void gravarBuff(){
  		File gravar;
      gravar = SD.open("SS.ss", FILE_WRITE);
      if(gravar){
        gravar.seek(sizeof(SequenceSet) + (buff->getEndereco() * sizeof(Bloco)));
        gravar.write((const char *) (buff), sizeof(SequenceSet));
      }
      gravar.close();
  	}
  	void gravarTxt(int valor){
      File gravar;
      gravar = SD.open("Out.txt", FILE_WRITE);
      if(gravar){
        gravar.print(valor);
        gravar.print(' ');
      }
      gravar.close();
    }
    void auxOutputFile(){
      Bloco* aux = new Bloco(ordem);
      File copiar;
      copiar = SD.open("SS.ss");
      File colar;
      colar = SD.open("aux.ss", FILE_WRITE);
      if(copiar and colar){
        copiar.seek(sizeof(SequenceSet));
        while(copiar.read((char *) (aux), sizeof(Bloco)))
          colar.write((char *) (aux), sizeof(Bloco));
      }
      copiar.close();
      colar.close();
    }
  	void getBlocoFromId(int id, Bloco& temp){
  		File ler;
      ler = SD.open("SS.ss", FILE_READ);
      if(ler){
        ler.seek(sizeof(SequenceSet) + (id * sizeof(Bloco)));
        ler.read((char *)(&temp), sizeof(Bloco));
      }
      ler.close();
  	}
    void getBlocoFromIdAux(int id, Bloco& temp){
      File ler;
      ler = SD.open("aux.ss", FILE_READ);
      if(ler){
        ler.seek(sizeof(SequenceSet) + (id * sizeof(Bloco)));
        ler.read((char *)(&temp), sizeof(Bloco));
      }
      ler.close();
    }
  	int enderecoValido(){
  		Bloco *temp = new Bloco(ordem);
  		File ler;
      int id  = 0;
      ler = SD.open("SS.ss", FILE_READ);
      if(ler){
        ler.seek(sizeof(SequenceSet));
        while(ler.read((char *) (&temp), sizeof(Bloco))){
          if(temp->getFlag() != '2'){
            delete temp;
            return id;
          }
          id++;
        }
      }
      delete temp;
      return id;
  	}
};
