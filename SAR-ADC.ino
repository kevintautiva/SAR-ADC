////// TOCA SINCRONIZAR LOS RELOJES
//Pines de salida se usarán para la palabra digital: Bit 0 en pin 0, Bit 1 en pin 1, ... , Bit 9 en pin 9
int D0 = 0;
// Numero de bits:
int N = 10;
// Pin para marcar final de la conversión (no es necesario, solo para pruebas)
int EOC = 10;
// Contador para saber cuando finaliza la conversión
int Count = 0;
//Define el pin de entrada del cual se leerá la salida del comparador
float PinEntrada = A0;
// Se da un valor inicial a los valores a usar
int Vread = 0;
float Vcom = 0;
int Vcc = 5;
// Arreglo para almacenar los bits
bool BitsAct[10]; /// Toca cambiar a 10 para la implementación, 8 para pruebas
//////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
 // Se definen los pines digitales como salida
 for (int i=0; i<=N+N-1; i++) {
  pinMode(i, OUTPUT);
 }
 pinMode(EOC, OUTPUT);
 //Se da una palabra digital inicial
 digitalWrite(N-1, HIGH);
 BitsAct[N-1]=HIGH;
 for (int i=0; i<=N-2; i++) {
  digitalWrite(i, LOW);
  BitsAct[i]=LOW;
 }
// Se pone que el final de la conversión aun no se da
 digitalWrite(EOC, LOW);
}
///////////////////////////////////////////////////////////////////////////////
void loop() {
  // Reinicio de la palabra digital a Vref/2 y marcar que no se ha finalizado la conversión
   if (Count == 0) {
   digitalWrite(EOC, LOW);
   digitalWrite(N-1, HIGH);
      for (int i=0; i<=N-2; i++) {
        digitalWrite(i, LOW);
      }
   }
  // Lee la entrada analógica y la convierte a un valor de voltaje (primero reinicio la palabra luego comparo)
  Vread = analogRead(PinEntrada);
  Vcom = Vread*Vcc/1023;
  //    delay(50); //para pruebas
  // Proceso de creación de la palabra digital para comparar (menos el ultimo bit) y guardado de la palabra en un vector
  if (Count<N-1) {
    if (Vcom>2.5) {
      digitalWrite(N-1-Count, HIGH);
      BitsAct[N-1-Count]=HIGH;
    }
    else {
      digitalWrite(N-1-Count, LOW);
      BitsAct[N-1-Count]=LOW;
    }
    digitalWrite(N-2-Count, HIGH);
    BitsAct[N-2-Count]=HIGH; //Pone el bit siguiente como 1
    Count=Count + 1;
  }
  // Asignación del ultimo bit de la palabra
  if (Vcom>=2.5 && Count==N-1) {
    digitalWrite(D0, HIGH);
    BitsAct[0]=HIGH;
  }
  else if (Vcom<2.5 && Count==N-1) {
    digitalWrite(D0, LOW);
    BitsAct[0]=LOW;
  }
  // Escritura de la palabra de salida (pines diferentes a los de la palabra para comparar), declaración del final de la conversión, reseteo del contador
  if (Count == N-1) {
    for (int i=0;i<=N-1;i++) {
      Serial.print("D");
      Serial.print(i);
      Serial.println(BitsAct[i]);
    }
    digitalWrite(EOC, HIGH);
    Serial.println("Fin de la conversión");
    Count = 0;
  }
}
