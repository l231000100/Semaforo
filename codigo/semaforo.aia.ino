/*
  Práctica: Semáforo Vehicular y Peatonal con Máquina de Estados Finitos
  Arduino UNO R4 WiFi

  - Sin delay() en ningún punto: toda la temporización usa millis().
  - Estados modelados con enum class (tipado fuerte, sin fugas al espacio global).
*/

// ---------- Pines ----------
const uint8_t PIN_VEH_ROJO     = 2;
const uint8_t PIN_VEH_AMARILLO = 3;
const uint8_t PIN_VEH_VERDE    = 4;
const uint8_t PIN_PEA_ROJO     = 5;
const uint8_t PIN_PEA_VERDE    = 6;
const uint8_t PIN_BOTON        = 7;

// ---------- Duraciones (ms) ----------
const unsigned long T_VEH_VERDE     = 8000;  // vehicular en verde
const unsigned long T_VEH_AMARILLO  = 2000;  // más corto que verde/rojo
const unsigned long T_VEH_ROJO      = 6000;  // vehicular en rojo (sin solicitud peatonal)
const unsigned long T_PEA_VERDE     = 5000;  // peatonal en verde
const unsigned long T_DEBOUNCE      = 50;    // antirrebote del botón

// ---------- Estados de la FSM ----------
enum class EstadoSemaforo {
  VEH_VERDE,
  VEH_AMARILLO,
  VEH_ROJO,
  PEA_VERDE
};

// ---------- Variables de estado global ----------
EstadoSemaforo estadoActual = EstadoSemaforo::VEH_VERDE;
unsigned long inicioEstado = 0;          // millis() en que se entró al estado actual
bool solicitudPeatonal = false;          // flag "armado" por el botón

// ---------- Antirrebote del botón ----------
// Se separan dos roles que antes estaban mezclados en una sola variable:
// - ultimaLecturaCruda: la última lectura física del pin, usada solo para
//   detectar cambios y reiniciar el temporizador de rebote.
// - estadoEstable: el último valor ya confirmado (estable durante T_DEBOUNCE),
//   usado para detectar el flanco de bajada real.
int ultimaLecturaCruda = HIGH;           // con INPUT_PULLUP, HIGH = suelto
int estadoEstable = HIGH;
unsigned long ultimoCambioLectura = 0;

// =====================================================================
// Apaga todos los LEDs y enciende solo los que corresponden al estado
// =====================================================================
void aplicarSalidasEstado(EstadoSemaforo estado) {
  digitalWrite(PIN_VEH_ROJO, LOW);
  digitalWrite(PIN_VEH_AMARILLO, LOW);
  digitalWrite(PIN_VEH_VERDE, LOW);
  digitalWrite(PIN_PEA_ROJO, LOW);
  digitalWrite(PIN_PEA_VERDE, LOW);

  switch (estado) {
    case EstadoSemaforo::VEH_VERDE:
      digitalWrite(PIN_VEH_VERDE, HIGH);
      digitalWrite(PIN_PEA_ROJO, HIGH);
      break;

    case EstadoSemaforo::VEH_AMARILLO:
      digitalWrite(PIN_VEH_AMARILLO, HIGH);
      digitalWrite(PIN_PEA_ROJO, HIGH);
      break;

    case EstadoSemaforo::VEH_ROJO:
      digitalWrite(PIN_VEH_ROJO, HIGH);
      digitalWrite(PIN_PEA_ROJO, HIGH);
      break;

    case EstadoSemaforo::PEA_VERDE:
      digitalWrite(PIN_VEH_ROJO, HIGH);   // el vehicular se mantiene en rojo
      digitalWrite(PIN_PEA_VERDE, HIGH);
      break;
  }
}

// =====================================================================
// Transición: cambia de estado, marca el tiempo de entrada y aplica
// las salidas correspondientes (acción de entrada del estado)
// =====================================================================
void cambiarEstado(EstadoSemaforo nuevoEstado) {
  estadoActual = nuevoEstado;
  inicioEstado = millis();
  aplicarSalidasEstado(nuevoEstado);
}

// =====================================================================
// Lee el botón con antirrebote. Devuelve true una sola vez, en el
// instante justo en que se detecta una pulsación válida (flanco de bajada).
// =====================================================================
bool botonPresionado() {
  int lectura = digitalRead(PIN_BOTON);
  bool presionDetectada = false;

  if (lectura != ultimaLecturaCruda) {
    ultimoCambioLectura = millis();
  }

  if ((millis() - ultimoCambioLectura) > T_DEBOUNCE) {
    // La lectura lleva estable más de T_DEBOUNCE: si cambió respecto
    // al último estado confirmado, es un cambio real (no rebote).
    if (lectura != estadoEstable) {
      estadoEstable = lectura;
      if (estadoEstable == LOW) {
        presionDetectada = true;
      }
    }
  }

  ultimaLecturaCruda = lectura;
  return presionDetectada;
}

// =====================================================================
// setup()
// =====================================================================
void setup() {
  pinMode(PIN_VEH_ROJO, OUTPUT);
  pinMode(PIN_VEH_AMARILLO, OUTPUT);
  pinMode(PIN_VEH_VERDE, OUTPUT);
  pinMode(PIN_PEA_ROJO, OUTPUT);
  pinMode(PIN_PEA_VERDE, OUTPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);

  cambiarEstado(EstadoSemaforo::VEH_VERDE);
}

// =====================================================================
// loop()
// =====================================================================
void loop() {
  unsigned long ahora = millis();
  unsigned long transcurrido = ahora - inicioEstado;

  // --- Lectura del botón: solo "arma" la solicitud si el vehicular
  //     está en VERDE o AMARILLO (requisito 3) ---
  if (botonPresionado()) {
    if (estadoActual == EstadoSemaforo::VEH_VERDE ||
        estadoActual == EstadoSemaforo::VEH_AMARILLO) {
      solicitudPeatonal = true;
    }
    // Si se presiona en otro estado, simplemente se ignora.
  }

  // --- Lógica de transición de la FSM ---
  switch (estadoActual) {

    case EstadoSemaforo::VEH_VERDE:
      if (transcurrido >= T_VEH_VERDE) {
        cambiarEstado(EstadoSemaforo::VEH_AMARILLO);
      }
      break;

    case EstadoSemaforo::VEH_AMARILLO:
      if (transcurrido >= T_VEH_AMARILLO) {
        cambiarEstado(EstadoSemaforo::VEH_ROJO);
      }
      break;

    case EstadoSemaforo::VEH_ROJO:
      if (solicitudPeatonal) {
        // Hay solicitud pendiente: en cuanto el vehicular llega a rojo,
        // se atiende de inmediato (requisito 4).
        solicitudPeatonal = false;
        cambiarEstado(EstadoSemaforo::PEA_VERDE);
      } else if (transcurrido >= T_VEH_ROJO) {
        // Nadie pidió cruzar: el ciclo sigue solo (requisito 5).
        cambiarEstado(EstadoSemaforo::VEH_VERDE);
      }
      break;

    case EstadoSemaforo::PEA_VERDE:
      if (transcurrido >= T_PEA_VERDE) {
        cambiarEstado(EstadoSemaforo::VEH_VERDE);
      }
      break;
  }
}
