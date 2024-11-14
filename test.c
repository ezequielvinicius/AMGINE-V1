#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pinos do 74HC595
int latchPin = 12; // registrador de saída
int dataPin = 13; // entrada 74HC595
int clockPin = 11; // registrador de deslocamento
int selectB = 2;
int i;

// Variável global para controlar a posição atual na 2ª linha do LCD
int posicaoLetraCifrada = 8;

// Quantidade de registradores
int qtdRegistradores = 4;

// valores limites em ambiente ideal
int V[] = {929, 852, 786, 730, 681, 639, 601, 568, 538, 511, 487, 465, 444, 426, 409, 393,
           379, 365, 352, 341, 330, 319, 310, 301, 292, 284, 276, 269, 262, 255, 249, 243};


// Vetores de posição, rotores, refletor
int vogaisPosicao[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
int vogaisTrocada1[] = { 5, 20, 19, 8, 7, 0, 21, 4, 3, 23, 11, 10, 25, 24, 15, 14, 22, 18, 17, 2, 1, 6, 16, 9, 13, 12 };
int vogaisTrocada2[] = { 20, 8, 11, 18, 12, 22, 23, 21, 1, 24, 17, 2, 4, 15, 25, 13, 19, 10, 3, 16, 0, 7, 5, 6, 9, 14 };
int vogaisTrocada3[] = { 9, 2, 1, 17, 22, 24, 16, 21, 15, 0, 23, 20, 14, 19, 12, 8, 6, 3, 25, 13, 11, 7, 4, 10, 5, 18 };
int vogaisTrocadaRefletor[] = { 9, 11, 8, 22, 14, 7, 21, 5, 2, 0, 20, 1, 17, 16, 4, 25, 13, 12, 23, 24, 10, 6, 3, 18, 19, 15 };

// Variaveis de apoio para troca de posição das letras e dos rotores
int posTrocada1, posTrocada2, posTrocada3, posTrocadaRefletor;
int rotor1Posicao, rotor2Posicao, rotor3Posicao = 0;
char letraRefletor;
char letraFinal;

char alphaValues[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
                        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int tamanho = 26;
byte ledsState[4] = {0}; // Array para armazenar o estado dos LEDs

LiquidCrystal_I2C lcd(0x27, 16, 2);

void clearScrn()
{
  lcd.clear(); // Limpa a tela do LCD
}
void setup()
{
  Wire.begin();  
  // Inicialização do LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PROJETO AMGINE");
  lcd.setCursor(0, 1);
  lcd.print("ADS 24/1 <> CODE");
   delay(300);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("ROTOR->");

 

  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(selectB, INPUT);

  lcd.setCursor(0, 1); // Define o cursor na segunda linha
}
void shiftWrite(byte *ledsState)
{
  digitalWrite(latchPin, LOW);
  for (i = qtdRegistradores - 1; i >= 0; i--)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, ledsState[i]);
  }
  digitalWrite(latchPin, HIGH);
}
void rotacionaArray(int arr[], int size) {
    int temp = arr[0];
    int i;
    for ( i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    arr[size - 1] = temp;
}
void rotacionaArrayInverso(int arr[], int size) {
    int temp = arr[size - 1];
    int i;
    for (i = size - 1; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
    arr[0] = temp;
}


void loop()
{
  int valor = analogRead(A1);
  float tensao = (valor * 5.0) / 1024.0;
  int ledIndex = -1;
  char letra = '0';
  int posInicial = -1;
  int limpar = digitalRead(2);

  if (valor < V[25] && valor >= V[26]) //Configuração Rotor 1 botão +
  {
    rotor1Posicao += 1;
    rotacionaArray(vogaisTrocada1, 26);
    if (rotor1Posicao > 25){rotor1Posicao = 0;}
    
  }
  else if (valor < V[26] && valor >= V[27]) //Configuração Rotor 1 botão -
  {
    rotor1Posicao-=1;
    rotacionaArrayInverso(vogaisTrocada1, 26);
    if (rotor1Posicao < 0){rotor1Posicao = 25;}
  }

  if (valor < V[27] && valor >= V[28]) //Configuração Rotor 2 botão +
  {
    rotor2Posicao+=1;
    rotacionaArray(vogaisTrocada2, 26);
    if (rotor2Posicao > 25){rotor2Posicao = 0;}
  }
  else if (valor < V[28] && valor >= V[29]) //Configuração Rotor 2 botão -
  {
    rotor2Posicao-=1;
    rotacionaArrayInverso(vogaisTrocada2, 26);
    if (rotor2Posicao < 0){rotor2Posicao = 25;}
  }

  if (valor < V[29] && valor >= V[30]) //Configuração Rotor 3 botão +
  {
    rotor3Posicao+=1;
    rotacionaArray(vogaisTrocada3, 26);
    if (rotor3Posicao > 25){rotor3Posicao = 0;}
  }
  else if (valor < V[30] && valor >= V[31]) //Configuração Rotor 3 -
  {
    rotor3Posicao-=1;
    rotacionaArrayInverso(vogaisTrocada3, 26);
    if (rotor3Posicao < 0){rotor3Posicao = 25;}
  }



  for (int i = 0; i < 26; i++) // identificação da letra digitada para ser impressa no LCD
  {
    if ((valor >= V[i]) && (valor > V[26]))
    {
      ledIndex = i;
      letra = alphaValues[i];
      
      break;
    }
  }
  for ( i = 0; i < tamanho; i++) { // registro da letra para ser codificada
        if (alphaValues[i] == letra) {
            posInicial = i;
            break;
        }
    }

  
  if (letra != '0') // letra digitada impressa na LCD
  {
    lcd.setCursor(0, 1);
    lcd.print(letra);
  }
  if ((valor >= V[i]) && (valor > V[26])){ // codificação da letra passando pelos rotores na ida e volta
    posTrocada1 = vogaisTrocada1[posInicial];
    posTrocada2 = vogaisTrocada2[posTrocada1];
    posTrocada3 = vogaisTrocada3[posTrocada2];
    posTrocadaRefletor = vogaisTrocadaRefletor[posTrocada3];
    letraRefletor = alphaValues[posTrocadaRefletor]; // troca da letra no refletor
	
  for (i = 0; i<tamanho; i++){ // codificação da letra passando pelos rotores na volta Rotor 3
		if (vogaisTrocada3[i] == posTrocadaRefletor){
		posTrocada3 = i;
		}
	}
  for (i = 0; i<tamanho; i++){ // codificação da letra passando pelos rotores na volta Rotor 2
		if (vogaisTrocada2[i] == posTrocada3){
		posTrocada2 = i;
		}
	}
  for (i = 0; i<tamanho; i++){ // codificação da letra passando pelos rotores na volta Rotor 1
		if (vogaisTrocada1[i] == posTrocada2){
		posTrocada1 = i;
		}
	}

  letraFinal = alphaValues[posTrocada1]; // letra final após todas as trocas
    
    lcd.setCursor(posicaoLetraCifrada, 1);
    //lcd.print(letraFinal);
    posicaoLetraCifrada++;
     if (posicaoLetraCifrada > 16) { // impressão sequencia de 8 caracteres de letras codificadas no LCD
      posicaoLetraCifrada = 8;
      lcd.setCursor(8, 1);
      lcd.print(letraFinal);
      posicaoLetraCifrada++;
      lcd.setCursor(posicaoLetraCifrada, 1);
      lcd.print("       ");
      
    }lcd.print(letraFinal);
  if (rotor1Posicao < 25) {  // Rotação dos Rotores após digitar e trocar a letra
                rotacionaArray(vogaisTrocada1, 26);
                rotor1Posicao++;
            } else {
                rotor1Posicao = 0;
                rotacionaArray(vogaisTrocada1, 26);
                rotacionaArray(vogaisTrocada2, 26);
                rotor2Posicao++;

                if (rotor2Posicao == 26) {
                    rotor2Posicao = 0;
                    rotacionaArray(vogaisTrocada1, 26);
	                  rotacionaArray(vogaisTrocada2, 26);
	                  rotacionaArray(vogaisTrocada3, 26);
                    rotor3Posicao ++;
                }
            }

  }
  if ((valor >= V[i]) && (valor > V[26])){ // Definindo o LED correspondente a letra trocada
ledIndex = posTrocada1;
  }
  
  for (int i = 0; i < tamanho; i++) // estado dos LED's
  {

    int byteIndex = i / 8;
    int bitIndex = i % 8;

    if (i == ledIndex)
    {
      ledsState[byteIndex] |= (1 << bitIndex); // Liga o LED
    }
    else
    {
      ledsState[byteIndex] &= ~(1 << bitIndex); // Desliga o LED
    }
  }

  shiftWrite(ledsState);

  if (limpar == HIGH){
    lcd.setCursor(0, 1);
    lcd.print("                ");
    posicaoLetraCifrada = 8;
    delay(200);
  }


  lcd.setCursor(8, 0);
  lcd.print(alphaValues[rotor3Posicao]);
  lcd.setCursor(11, 0);
  lcd.print(alphaValues[rotor2Posicao]);
  lcd.setCursor(14, 0);
  lcd.print(alphaValues[rotor1Posicao]);

  Serial.print("Valor: ");
  Serial.print(valor);
  Serial.print("    Pino: ");
  Serial.print(ledIndex +1 );
  Serial.print("    Tensao: ");
  Serial.print(tensao);
  Serial.print("    Letra: ");
  Serial.println(letra);

  delay(30);
}
