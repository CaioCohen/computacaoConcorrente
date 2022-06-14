import java.util.Random;

class Preenche implements Runnable {
    private int indice;
    int[] vetor;
    int N;

    // --construtor
    public Preenche(int i, int[] vetor, int N) {
        this.indice = i;
        this.vetor = vetor;
        this.N = N;
    }

    // --metodo executado pela thread
    public void run() {
        Random rand = new Random();
        for (int i = this.indice; i < this.vetor.length; i += this.N) {
            this.vetor[i] = rand.nextInt(10);
        }
    }
}

class EncontrarPares extends Thread {
    private int indice;
    int[] vetor;
    int N;
    Pares numeroPares;

    // --construtor
    public EncontrarPares(int i, int[] vetor, int N, Pares numeroPares) {
        this.indice = i;
        this.vetor = vetor;
        this.N = N;
        this.numeroPares = numeroPares;
    }

    // --metodo executado pela thread
    public void run() {
        int numero = 0;
        for (int i = this.indice; i < this.vetor.length; i += this.N) {
            if (vetor[i] % 2 == 0) {
                numero++;
            }
        }
        this.retornarSoma(numero);
    }

    public synchronized void retornarSoma(int soma) {
        this.numeroPares.addPares(soma);
    }
}

class Pares{
    public int numeroPares;
    public Pares(){
        this.numeroPares = 0;
    }

    public void addPares(int n){
        this.numeroPares += n;
    }

    public int getPares(){
        return this.numeroPares;
    }
}

public class App {
    static final int N = 4;
    static final int Tamanho = 1000000;

    public static void main(String[] args) {
        Thread[] threads = new Thread[N];
        int[] vetor = new int[Tamanho];
        Pares NumeroPares = new Pares();

        for (int i = 0; i < threads.length; i++) {
            threads[i] = new Thread(new Preenche(i, vetor, N)); // Eu sei que o lab pede apenas para que a leitura seja
                                                                // feita concorrente, mas para aumentar a performance,
                                                                // fiz o preenchimento concorrente também
        }

        // --PASSO 3: iniciar a thread
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        // --PASSO 4: esperar pelo termino das threads
        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }

        for (int i = 0; i < threads.length; i++) {
            threads[i] = new EncontrarPares(i, vetor, N, NumeroPares); // por algum motivo, ele não deixa o
                                                                                   // NumeroPares não ser estatico
        }

        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }
        System.out.println(NumeroPares.getPares());//por algum motivo o valor de NumeroPares não está mudando
    }
}
