namespace LIGHTS{
  int pin_red = 2;
  int pin_blue = 4;
  int pin_green = 3;

  enum COLORS {
    C_RED = 0,
    C_GREEN,
    C_BLUE,
    C_YELLOW,
    C_TEAL,
    C_PINK,
    C_WHITE,
  };

  void Red(bool enabled = true){
    this.Clear();
    digitalWrite(pin_red, (enabled ? HIGH : LOW));
  }
  
  void Green(bool enabled = true){
    digitalWrite(pin_green, (enabled ? HIGH : LOW));
  }
  
  void Blue(bool enabled = true){
    digitalWrite(pin_blue, (enabled ? HIGH : LOW));
  }

  void Yellow(bool enabled = true){
    digitalWrite(pin_red, (enabled ? HIGH : LOW));
    digitalWrite(pin_green, (enabled ? HIGH : LOW));
  }

  void Teal(bool enabled = true){
    digitalWrite(pin_blue, (enabled ? HIGH : LOW));
    digitalWrite(pin_green, (enabled ? HIGH : LOW));
  }

  void Pink(bool enabled = true){
    digitalWrite(pin_red, (enabled ? HIGH : LOW));
    digitalWrite(pin_blue, (enabled ? HIGH : LOW));
  }

  void White(bool enabled = true){
    digitalWrite(pin_red, (enabled ? HIGH : LOW));
    digitalWrite(pin_green, (enabled ? HIGH : LOW));
    digitalWrite(pin_blue, (enabled ? HIGH : LOW));
  }
  
  void Clear(){
    Red(false);
    Green(false);
    Blue(false);
  }

  void SetColor(COLORS color){
    Clear();
    switch(color){
      case C_RED: Red(); break;
      case C_GREEN: Green(); break;
      case C_BLUE: Blue(); break;
      case C_YELLOW: Yellow(); break;
      case C_TEAL: Teal(); break;
      case C_PINK: Pink(); break;
      case C_WHITE: White(); break;
    }
  }
}
namespace BUTTONS{
  const int pin_one = 6;
  const int pin_two = 7;
  const int pin_three = 8;
  
  bool ReadOne(){
    return digitalRead(pin_one) != HIGH;
  }

  bool ReadTwo(){
    return digitalRead(pin_two) != HIGH;
  }

  bool ReadThree(){
    return digitalRead(pin_three) != HIGH;
  }

  bool ButtonOneReleased() {
    static bool bOneHeld = false;

    if(ReadOne()){
      bOneHeld = true;
    }
    else if(bOneHeld){
      return true;
    }

    return false;
  }

  bool ButtonTwoReleased() {
    static bool bTwoHeld = false;

    if(ReadTwo()){
      bTwoHeld = true;
    }
    else if(bTwoHeld){
      return true;
    }
    return false;
  }

  bool ButtonThreeReleased() {
    static bool bThreeHeld = false;

    if(ReadThree()){
      bThreeHeld = true;
    }
    else if(bThreeHeld){
      return true;
    }
    return false;
  }
}

#define SIMON_COUNT 12
int simon_colors[SIMON_COUNT] = {};

int currentStep = 0;
int currentMicroStep = 0;
bool displaySimon = true;
bool failed = false;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  pinMode(BUTTONS::pin_one, INPUT);
  pinMode(BUTTONS::pin_two, INPUT);
  pinMode(BUTTONS::pin_three, INPUT);
  
  pinMode(LIGHTS::pin_red, OUTPUT);
  pinMode(LIGHTS::pin_green, OUTPUT);
  pinMode(LIGHTS::pin_blue, OUTPUT);
  LIGHTS::Clear();
  
  for(int i = 0; i < SIMON_COUNT; i++){
    simon_colors[i] = random(LIGHTS::C_RED, LIGHTS::C_BLUE + 1);
    if(simon_colors[i] == 0)
      Serial.println("RED");
    else if(simon_colors[i] == 1)
      Serial.println("GREEN");
    else if(simon_colors[i] == 1)
      Serial.println("BLUE");
  }
}
void DisplaySteps(){
  LIGHTS::Clear();
  if(displaySimon){
    for(int i = 0; i < currentStep + 1; i++){
      LIGHTS::SetColor(simon_colors[i]);
      delay(500);
      LIGHTS::Clear();
      delay(200);
    }
    displaySimon = false;
  }
}

void Correct(){
  Serial.println("Correct");
  
  if(currentMicroStep == currentStep){
    // Change to new level
    currentStep++;
    currentMicroStep = 0;
    displaySimon = true;

    LIGHTS::SetColor(LIGHTS::C_WHITE);
  }else{
    // Continue on same level
    LIGHTS::SetColor(simon_colors[currentMicroStep]);
    currentMicroStep++;
  }
  delay(500);
  LIGHTS::Clear();
}
void Wrong(){
  Serial.println("Wrong");
  failed = true;
  for(int i = 0; i < 3; i ++){
    LIGHTS::Yellow();
    delay(500);
    LIGHTS::Clear();
    delay(250);
  }
}

bool CheckCorrect(int button){
  if(button == simon_colors[currentMicroStep]){
    Correct();
  }else{
    Wrong();
  }
}

void CheckButtons(){
  #define BUTTON_COUNT 3
  static bool bPrevHeld[] = { false, false, false };

  bool bButtonHeld[] = { BUTTONS::ReadOne(), BUTTONS::ReadTwo(), BUTTONS::ReadThree() };
  for(int i = 0; i < BUTTON_COUNT; i++){
    if(bButtonHeld[i]){
      bPrevHeld[i] = true;
    }else if(bPrevHeld[i]){
      CheckCorrect(i);
      bPrevHeld[i] = false;
    }
  }
}

void loop() {
  if(failed)
    return;

  DisplaySteps();
  CheckButtons();
}
