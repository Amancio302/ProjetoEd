class Bloco{
  public:
    Bloco(int tam){
      endereco = -1;
      proximo = -1;
      anterior = -1;
      flag = '0';
      quantidade = 0;
      quantidadeMax = tam;
     // dados = new int[quantidadeMax];
    }
    ~Bloco(){
      //delete[] dados;
    }
    void inserir(int valor){
      int pos = posicaoPara(valor);
      empurraDireita(pos);
      dados[pos] = valor;
      quantidade++;
      if(quantidade == quantidadeMax)
        setCheio();
    }
    void remover(int valor){
      int pos = buscar(valor);
      if(pos != -1){
        empurraEsquerda(pos);
        if(quantidade == quantidadeMax)
          setDisponivel();
        quantidade--;
      }
    }
    int removerPos(int pos){
      if(pos < quantidade and pos >= 0){
        int valor = dados[pos];
        empurraEsquerda(pos);
        if(quantidade == quantidadeMax)
          setDisponivel();
        quantidade--;
        return valor;
      }
      return -1;
    }
    int getValor(int pos){
      if(pos < quantidade and pos >= 0)
        return dados[pos];
      return -1;
    }
    int buscar(int valor){
      for(int i = 0; i < quantidade; i++)
        if(dados[i] == valor)
          return i;
      return -1;
    }
    void setEndereco(int ender){
      endereco = ender;
    }
    void setProximo(int prox){
      proximo = prox;
    }
    void setAnterior(int ant){
		anterior = ant;
	}
    void setVazio(){
		flag = '0';
	}
    void setDisponivel(){
      flag = '1';
    }
    void setCheio(){
      flag = '2';
    }
    void setRemovido(){
      flag = '3';
    }
    int getEndereco(){
		return endereco;
	}
    int getProximo(){
      return proximo;
    }
    int getAnterior(){
		return anterior;
	}
    char getFlag(){
      return flag;
    }
    int getQuantidade(){
      return quantidade;
    }
  private:
    int endereco;
    int proximo;
    int anterior;
    char flag;
    int quantidade;
    int quantidadeMax;
    int dados[10];

    bool empurraDireita(int pos){
      for(int i = quantidade; i > pos; i--)
        dados[i] = dados[i-1];
    }
    bool empurraEsquerda(int pos){
      for(int i = pos; i < quantidade; i++)
        dados[i] = dados[i+1];
    }
    int posicaoPara(int valor){
      for(int i = 0; i < quantidade; i++)
        if(valor < dados[i])
          return i;
      return quantidade;
    }
};
