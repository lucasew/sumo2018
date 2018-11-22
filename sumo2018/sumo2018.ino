#include <Ultrasonic.h>

#define FRENTE 0
#define TRAS 1
#define DIREITA 0
#define ESQUERDA 1

struct Motor {
  int A1;
  int A2;
};

void andar(struct Motor motor, int direcao, int high) {
  //high = HIGH;
  analogWrite(motor.A1, direcao*high);
  analogWrite(motor.A2, !direcao*high);
}

// Remapeia e força os valores dentro do intervalo
#define REMAP(in, x, y, a, b) constrain(map(in, x, y, a, b), a, b)
#define BOOL2DIGITAL(x) x ? 255 : 0

Ultrasonic ufrente(8, 9); // Trigger e então echo
Ultrasonic utras(10, 11);
struct Motor mdireito = {7, 6};
struct Motor mesquerdo = {5, 4};
// 7, 6, 5, 4 são pins da ponte h

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando....");
  delay(5000);
}
int US_frente = 0;
int US_tras = 0;
int perto_frente = 0;
int perto_tras = 0;
int lado = 0;

void update_states() {
  US_frente = ufrente.Timing(); // Pega o delay de bater e voltar
  Serial.print("Frente: ");
  Serial.print(US_frente);
  US_tras = utras.Timing();
  Serial.print(" Trás: ");
  Serial.println(US_tras); 
  perto_frente = US_frente < 1500;
  perto_tras = US_tras < 1500;
  lado = millis()%2; // Escravizar como gerador de entropia :v
}

int decidir_lado() { // Medida tomada para o carrinho não viciar para lado nenhum, pois se o if passar o else não chega a ser executado
  if (perto_frente) {
    andar(mdireito, TRAS, 255);
    andar(mesquerdo, TRAS, 255);
  }
  if (perto_tras) {
    andar(mdireito, FRENTE, 255);
    andar(mesquerdo, FRENTE, 255);
  }
  return perto_frente || perto_tras;
}

void loop() {
  update_states();
  if (!decidir_lado()) {
    andar(mdireito, lado, 100);
    andar(mesquerdo, !lado, 100);
    delay(200);
  } else {
    delay(100);
  }
  delay(300);
  Serial.println(lado);
}
