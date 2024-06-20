/********************************************************************************************
Robot CRC 2024 André Laurendeau

IMPORTANT : NE PAS AVANCER ET UTILISER LE LIFT + LAUNCHER

Pin PWM:
  1: Moteur drive gauche (-128,127)
  2: Moteur Launcher x2 (0,-128)
  3: Fan x3 (0,127)
  4: 
  5: mort??? //Neo Pixel x3
  6: 
  7:
  8: Neo Pixel x3
  9: Moteur Lift (127,-128)
  10: Moteur Kirby (-128,127)
  11: Moteur Strap (-128,127)
  12: Moteur drive droite (-128,127)

Pin DIG:
  1: Limit switch entrée Kirby
  2: Limit switch sortie Kirby (position 1)
  3: Limit switch entrée Launcher
  4:
  5:
  6:
  7:
  8:
  9:
  10:
  11:
  12:

Pin Analog :
  1:
  2:
  3:
  4:

Pin Encodeur:
  i2c:
    SDA: Encodeur Launcher A
    SCL: Encodeur Launcher B
  Serial :
    TXD1:
    RXD1:
  Encodeur:  de 0 a -2480
    Encodeur Lift A
    Encodeur Lift B
  SPI:
    MISO:
    MOSI:
    SCK:
    SS:

Controle port série:
  2 pour basculer entre le Set_Point_Serial et SET_Point_RPM
  1xxxx Remplacer x par le SET_Point_RPM (Serial) voulu

Controle Manette:
  Analog:
    JoyStick Gauche : Drive
    JoyStick Droit : Lift en mode PID
    L2 : Launcher en mode manuel
    R2 : Lift en mode manuel

  Digital:
    Pour les modes (combinaison START(appuyer en premier)+:
      D PAD UP : Launcher PID/Manuel
      D PAD LEFT : Strap Auto/Manuel
      D PAD RIGHT : Child mode
      D PAD DOWN : Lift PID/Manuel
    
    Le reste des boutons:
        D PAD UP : SET_RPM++
        D PAD LEFT : Launcher On/Off
        D PAD RIGHT : Launcher Standby/Spin up 
        D PAD DOWN : SET_RPM--

        Color UP : Fan On/Off
        Color LEFT : Override Strap avancer
        Color RIGHT : Override Strap reculer
        Color DOWN : Kirby sortir
        
        L1 : Lancer ballon une fois le RPM SET dépasser
        R1 : Kirby prendre et pour le changement de mode appuyer en premier

        Start : Combinaison changement de mode
        Select: Changement mode Serial RPM
        Logo: Child mode

        HATL:
        HATR:

Commande port série:
  2 : changement entre le mode série RPM et robot normal
  1xxxx : Remplacer x par la valeur RPM voulue

DEL 1 (coté Lift):
  Affiche le voltage de la batterie
... (745 lines left)
Collapse
message.txt
27 KB
﻿
CHPOUTNIK
chpoutnik
/********************************************************************************************
Robot CRC 2024 André Laurendeau

IMPORTANT : NE PAS AVANCER ET UTILISER LE LIFT + LAUNCHER

Pin PWM:
  1: Moteur drive gauche (-128,127)
  2: Moteur Launcher x2 (0,-128)
  3: Fan x3 (0,127)
  4: 
  5: mort??? //Neo Pixel x3
  6: 
  7:
  8: Neo Pixel x3
  9: Moteur Lift (127,-128)
  10: Moteur Kirby (-128,127)
  11: Moteur Strap (-128,127)
  12: Moteur drive droite (-128,127)

Pin DIG:
  1: Limit switch entrée Kirby
  2: Limit switch sortie Kirby (position 1)
  3: Limit switch entrée Launcher
  4:
  5:
  6:
  7:
  8:
  9:
  10:
  11:
  12:

Pin Analog :
  1:
  2:
  3:
  4:

Pin Encodeur:
  i2c:
    SDA: Encodeur Launcher A
    SCL: Encodeur Launcher B
  Serial :
    TXD1:
    RXD1:
  Encodeur:  de 0 a -2480
    Encodeur Lift A
    Encodeur Lift B
  SPI:
    MISO:
    MOSI:
    SCK:
    SS:

Controle port série:
  2 pour basculer entre le Set_Point_Serial et SET_Point_RPM
  1xxxx Remplacer x par le SET_Point_RPM (Serial) voulu

Controle Manette:
  Analog:
    JoyStick Gauche : Drive
    JoyStick Droit : Lift en mode PID
    L2 : Launcher en mode manuel
    R2 : Lift en mode manuel

  Digital:
    Pour les modes (combinaison START(appuyer en premier)+:
      D PAD UP : Launcher PID/Manuel
      D PAD LEFT : Strap Auto/Manuel
      D PAD RIGHT : Child mode
      D PAD DOWN : Lift PID/Manuel
    
    Le reste des boutons:
        D PAD UP : SET_RPM++
        D PAD LEFT : Launcher On/Off
        D PAD RIGHT : Launcher Standby/Spin up 
        D PAD DOWN : SET_RPM--

        Color UP : Fan On/Off
        Color LEFT : Override Strap avancer
        Color RIGHT : Override Strap reculer
        Color DOWN : Kirby sortir
        
        L1 : Lancer ballon une fois le RPM SET dépasser
        R1 : Kirby prendre et pour le changement de mode appuyer en premier

        Start : Combinaison changement de mode
        Select: Changement mode Serial RPM
        Logo: Child mode

        HATL:
        HATR:

Commande port série:
  2 : changement entre le mode série RPM et robot normal
  1xxxx : Remplacer x par la valeur RPM voulue

DEL 1 (coté Lift):
  Affiche le voltage de la batterie
    Doit absolument etre vert avant un match et changer avant d'etre orange
    flash rouge c'est plus que le temps d'arreter

DEL 2 (millieu):
  Mode du robot:
    Rouge qui flash : manette déconnecter
      Ne flash pas quand en Child mode:
        Orange : Tout est en full auto
        Mauve : Tout sauf Lift en mode Manuel
        Bleu Ciel : Tout sauf le Launcher en mode Manuel
        Jaune : Tout sauf la strap en mode Manuel
        Vert : Juste le Lift en PID
        Bleu : Juste la strap en Automatique
        Rose : Juste le Launcher
        Blanc : Tout en Manuel
    AUCUNE INDICATION POUR MODE SERIAL RPM


DEL 3 (coté lanceur):
  Launcher off : flash rouge et fermer
  En stanby (2000 RPM):
    (Pour le nombre de ballon) Flash blanc pas fort et :
    Fermer : 0
    Rose pale : 1
    rose : 2
    Rose plus foncé : 3
    Bleu : 4
    Bleu ciel : 5
    Vert : 6 
    Jaune :7
    Orange : 8
    Rouge : 9
    Blanc : 10
  
  Launcher On SET_Point_RPM (SetPoint atteint Flash de la meme couleur):
    Rose pale : 3500
    rose : 4000
    Rose plus foncé : 4500
    Bleu : 5000
    Bleu ciel : 5500
    Vert : 6000
    Jaune :6500
    Orange : 7000
    Rouge : 7500


// Toujours problème avec start des PID
***********************************************************************************************/
#include <XBee.h>  //https://engineerforbeer.wordpress.com/2014/03motor1Enco/12/multiwii-2570quadcopter-xbox-controller-via-xbee/
#include <CrcLib.h>
#include "CrcRemoteState.h"
#include <Encoder.h>
#include "PID_RT.h"
#include <FastLED.h>

// NEO PIXEL
#define DATA_PIN CRC_PWM_8  // 5 mort???
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];
int Array_DEL[3] = { 0, 0, 0 };  // Pour fonction DELx9


int ballCount = 3;


Encoder Enco_Lift(CRC_ENCO_A, CRC_ENCO_B);
Encoder Enco_Launcher(CRC_I2C_SDA, CRC_I2C_SCL);


PID_RT PID_Lift;
PID_RT PID_Launcher;

//blink state
bool Blink_MODE = false;
bool Blink_Bat = false;
bool Blink_Launcher = false;
unsigned long Refresh_Del;  // valeur millis() du dernier refresh

// valeur des encodeur
long Enco_Lift_V;
long Enco_Launcher_V;
long Enco_Launcher_V_Ancienne = 0;

// analog
int joy1_Y;
int joy1_X;
int joy2_Y;
uint8_t LeftTrig;

// Pour les personnes qui savent pas pilote
int Child_1 = 127;
int Child_2 = -128;
bool Child = false;


unsigned long delai_RPM = 0;  // valeur millis() du dernier calcul RPM
int RPM = 0;
int SET_RPM = 6;         // 0 a 9 pour choisir entre les 9 option de SET_Point_RPM (départ a 6000 rpm bon compromis d'un peu tout)
int SET_RPM_TEMPORAIRE;  // RPM Serial
//                         Stanby,1,2,3,4,5,6,7,8,9
int SET_Point_RPM[10] = { 2000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500 };


/****************
PID on/manuel 
strap auto/manuel
*****************/
bool Launcher_PID = true;
bool Lift_PID = true;
bool Carosse_Auto = true;
bool Fan = true;
bool Launcher_On = false;
bool Launcher_Stanby = true;

// lire port série format pour écrire vitesse launcher = 1ValRpm et 2 pour changer le mode Serial_RPM ou normale
bool Serial_RPM = false;  // mode
String receivedCommand;

// data Process de la lecture du port série
void dataProcessing(String data) {
  int idValue, intValue;

  String id = data.substring(0, 1);
  String value = data.substring(1, 5);

  idValue = id.toInt();
  intValue = value.toInt();

  switch (idValue) {
    case 1:  // ID1: pwm PID
      SET_RPM_TEMPORAIRE = intValue;
      Serial.println("RPM Setpoint 0 a 7500: ");
      Serial.println(SET_RPM_TEMPORAIRE);
      break;

    case 2:  // Modifier une prochaine valeur
      Serial_RPM = !Serial_RPM;
      Serial.print("Serial_RPM: ");
      Serial.println(Serial_RPM);
      Serial.println();
      break;

    default:
      Serial.println("Commande Invalide");
      break;
  }
}


//Lift auto/manuel
void Lift() {

  if (Lift_PID) {                                                                                            //mode PID
    PID_Lift.start();                                                                                        // Ajouter delais pour régler a cause de la manette
    joy2_Y = constrain(map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y), 127, -128, 0, -2600), -2480, 0);  //-2480 vrai max

    // si manette déco baisser doucement
    if (!CrcLib::IsCommValid()) {
      PID_Lift.setPoint(0);
    } else {
      PID_Lift.setPoint(joy2_Y);
    }

    if (PID_Lift.compute(Enco_Lift_V)) {
      CrcLib::SetPwmOutput(CRC_PWM_9, PID_Lift.getOutput());
      /*
    Serial.print(PID_Lift.getInput());
    Serial.print('\t');
    Serial.println(PID_Lift.getOutput());
    */
    }

    // mode Manuel
  } else {
    PID_Lift.stop();
    int RightTrig = map(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R), 127, -128, 127, -20);  // -20 pour déscente controllé
    CrcLib::SetPwmOutput(CRC_PWM_9, RightTrig);
    /* marche pas quand encodeur reset
    // arrete pour éviter de s'enrouler de l'autre coté
    if (Enco_Lift_V >= 0 && RightTrig <= 0) {
      CrcLib::SetPwmOutput(CRC_PWM_9, 0);
    } else {
      CrcLib::SetPwmOutput(CRC_PWM_9, RightTrig);
    }*/
  }
}

// Launcher auto/manuel/série
void Launcher() {

  if (Launcher_PID) {  // mode Launcher
    PID_Launcher.start();
    /*Serial.print(SET_RPM);
    Serial.print("\t");
    Serial.println(SET_Point_RPM[SET_RPM]);
    */

    // mesure au 100 millis
    if (millis() >= delai_RPM + 100) {

      RPM = (abs(Enco_Launcher_V - Enco_Launcher_V_Ancienne)) * 6000 / (millis() - delai_RPM);  // Environ RPM (off ≈ +1000) (abs regle le probleme quand recule)
      //Serial.println(RPM);
      Enco_Launcher_V_Ancienne = Enco_Launcher_V;


      delai_RPM = millis();
    }

    // dans le range RPM correct
    if (RPM >= 0 && RPM <= 8000) {
      if (Launcher_On && Serial_RPM) {
        PID_Launcher.setPoint(SET_RPM_TEMPORAIRE);
      } else if (Launcher_On && !Serial_RPM) {
        if (Launcher_Stanby) {
          PID_Launcher.setPoint(SET_Point_RPM[0]);  // 0 stanby 2000 RPM
        } else {
          PID_Launcher.setPoint(SET_Point_RPM[SET_RPM]);
        }
      } else {
        PID_Launcher.setPoint(0);
      }
    }

    if (PID_Launcher.compute(RPM)) {
      /*
      Serial.print(PID_Launcher.getInput());
      Serial.print('\t');
      Serial.print(PID_Launcher.getSetPoint());
      Serial.print('\t');
      Serial.print(PID_Launcher.getOutput());
      Serial.print('\t');
      Serial.println();
      */

      CrcLib::SetPwmOutput(CRC_PWM_2, -PID_Launcher.getOutput());  // - important pour inverser la valeurs du signal PWM (max -128 pour bonne direction)
    }

    //mode manuel
  } else {
    PID_Launcher.stop();
    LeftTrig = map(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L), -128, 127, 0, -128);
    CrcLib::SetPwmOutput(CRC_PWM_2, LeftTrig);
  }
}

unsigned long strapMillis;
const int delaiStrapExtra = 300;

// strap auto/manuel/override
void Carosse() {
  static byte CarosseCase = 1;
  static bool pressFlag, releaseFlag = false;
  static bool Previous_SWITCH3 = false;
  static unsigned long minuterieDBNC = 0;
  static byte delaiDBNC = 1;
  //Serial.println(ballCount);

  if (!Carosse_Auto || CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT) || CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT)) {  //Passe en mode Full manuel
    CarosseCase = 4;
  } else if (CrcLib::ReadDigitalChannel(BUTTON::L1)) {  //mode lancer
    CarosseCase = 3;
  } else if (CrcLib::GetDigitalInput(CRC_DIG_3)) {  //Si un ballon est au lanceur, passe en mode lanceur
    CarosseCase = 3;
  }

  switch (CarosseCase) {
    case 1:  //WAITING
      if (millis() - strapMillis > delaiStrapExtra) {
        CrcLib::SetPwmOutput(CRC_PWM_11, 0);  //Attends d'avoir une balle dans le kirby. Si oui, passe en mode avancement des balles
      }
      /*
      Serial.println("case 1");
      Serial.println(CrcLib::GetDigitalInput(CRC_DIG_1));
      */
      if (CrcLib::GetDigitalInput(CRC_DIG_1)) {
        if (ballCount < 4) {
          ballCount++;  //Incrémente le nombre de balle en magasin
        }
        CarosseCase++;
      }
      break;
    case 2:  //Tourne strap
      //flagMoteurStrap = true;
      CrcLib::SetPwmOutput(CRC_PWM_11, -128);  //Active la strap jusqu'à ce que le ballon soit assez poussé.
      //Serial.println("case 2");
      if (CrcLib::GetDigitalInput(CRC_DIG_2)) {
        pressFlag = true;
      }
      if (pressFlag == true) {
        if (!CrcLib::GetDigitalInput(CRC_DIG_2)) {
          releaseFlag = true;
        }
      }
      if (pressFlag && releaseFlag) {
        pressFlag = false;
        releaseFlag = false;
        CarosseCase = 1;
        strapMillis = millis();
      }
      break;
    case 3:  //Wait to shoot
      //Serial.println("Case 3");
      if (CrcLib::ReadDigitalChannel(BUTTON::L1) && RPM >= SET_Point_RPM[SET_RPM] && SET_RPM > 0) {  //Si le bouton est appuyé, active la strap
        CrcLib::SetPwmOutput(CRC_PWM_11, 128);

      } else if (CrcLib::ReadDigitalChannel(BUTTON::L1) && !CrcLib::GetDigitalInput(CRC_DIG_3) && RPM < SET_Point_RPM[SET_RPM] || SET_RPM == 0) {
        CrcLib::SetPwmOutput(CRC_PWM_11, 128);
      } else {
        CrcLib::SetPwmOutput(CRC_PWM_11, 0);
      }
      //retourne en mode ramassage
      CarosseCase = 1;

      break;
    case 4:                                                    // Mode Full Manuel
      if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT)) {  //Active la strap lorsque le bouton est appuyé
        CrcLib::SetPwmOutput(CRC_PWM_11, 127);
      } else if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT)) {
        CrcLib::SetPwmOutput(CRC_PWM_11, -128);
      } else {
        CrcLib::SetPwmOutput(CRC_PWM_11, 0);
      }
      CarosseCase = 1;
      break;
  }
  if (!CrcLib::GetDigitalInput(CRC_DIG_3) && Previous_SWITCH3 && millis() - minuterieDBNC >= delaiDBNC && ballCount > 0) {
    ballCount--;
  }
  Previous_SWITCH3 = CrcLib::GetDigitalInput(CRC_DIG_3);
}

int Couleur_DEL_x9(int Num_Case) {

  switch (Num_Case) {

    case 0:  // rien pas supposer arriver
      Array_DEL[0] = 0;
      Array_DEL[1] = 0;
      Array_DEL[2] = 0;
      break;
    case 1:
      Array_DEL[0] = 255;  //  rose
      Array_DEL[1] = 75;
      Array_DEL[2] = 75;
      break;
    case 2:
      Array_DEL[0] = 255;
      Array_DEL[1] = 0;
      Array_DEL[2] = 50;  // mauve pale
      break;
    case 3:
      Array_DEL[0] = 255;  // mauve
      Array_DEL[1] = 0;
      Array_DEL[2] = 255;
      break;
    case 4:
      Array_DEL[0] = 0;
      Array_DEL[1] = 0;
      Array_DEL[2] = 255;  // bleu
      break;
    case 5:
      Array_DEL[0] = 100;  // bleu ciel
      Array_DEL[1] = 100;
      Array_DEL[2] = 255;
      break;
    case 6:
      Array_DEL[0] = 0;  //vert
      Array_DEL[1] = 255;
      Array_DEL[2] = 0;
      break;
    case 7:
      Array_DEL[0] = 255;  // jaune
      Array_DEL[1] = 255;
      Array_DEL[2] = 0;
      break;
    case 8:
      Array_DEL[0] = 255;  // orange
      Array_DEL[1] = 40;
      Array_DEL[2] = 0;
      break;
    case 9:
      Array_DEL[0] = 255;  // rouge
      Array_DEL[1] = 0;
      Array_DEL[2] = 0;
      break;
    case 10:
      Array_DEL[0] = 255;
      Array_DEL[1] = 255;
      Array_DEL[2] = 255;
      break;
  }
}

void DEL() {
  // Refresh au 150 millis
  if (millis() >= Refresh_Del + 150) {

    /**************************************************
    DEL batterie
    **************************************************/
    float BatV = CrcLib::GetBatteryVoltage(1.014);
    int Bat_G = map(constrain(BatV, 12, 12.5) * 100, 12 * 100, 12.5 * 100, 0, 255);  //full allumer a 12.5 et diminue jusqu'a 11.7
    int Bat_R = map(constrain(BatV, 12.5, 13) * 100, 12.5 * 100, 13 * 100, 255, 0);  //Commence a allumer apres 13v et est full rouge a 12.5v et seule allumer a  11.7

    if (BatV <= 10) {  //niveau critique 10??? (plus fuze que V drop)
      if (Blink_Bat == false) {
        Blink_Bat = true;
        leds[0].setRGB(255, 0, 0);
      } else {
        Blink_Bat = false;
        leds[0].setRGB(0, 0, 0);
      }
    } else {
      leds[0].setRGB(Bat_R, Bat_G, 0);
    }


    /**************************************************
    DEL Mode (en cours de changement)
    **************************************************/
    if (!CrcLib::IsCommValid() && !Blink_MODE) {
      Blink_MODE = true;
      leds[1].setRGB(255, 0, 0);

    } else if (Launcher_PID && Lift_PID && Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(255, 40, 0);
      Blink_MODE = true;
    } else if (Launcher_PID && Lift_PID && Carosse_Auto && Child) {
      leds[1].setRGB(255, 40, 0);
    } else if (Launcher_PID && Lift_PID && !Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  //Jaune
      Blink_MODE = true;
    } else if (Launcher_PID && Lift_PID && !Carosse_Auto && Child) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  //Jaune
    } else if (Launcher_PID && !Lift_PID && Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  //Rose(foncé)
      Blink_MODE = true;
    } else if (Launcher_PID && !Lift_PID && Carosse_Auto && Child) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  //Rose(foncé)
    } else if (!Launcher_PID && Lift_PID && Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  // Bleu pale
      Blink_MODE = true;
    } else if (!Launcher_PID && Lift_PID && Carosse_Auto && Child) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  // Bleu pale
    } else if (!Launcher_PID && Lift_PID && !Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  // Vert
      Blink_MODE = true;
    } else if (!Launcher_PID && Lift_PID && !Carosse_Auto && Child) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  // Vert
    } else if (!Launcher_PID && !Lift_PID && Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  //Bleu
      Blink_MODE = true;
    } else if (!Launcher_PID && !Lift_PID && Carosse_Auto && Child) {
      leds[1].setRGB(Launcher_PID * 255, Lift_PID * 255, Carosse_Auto * 255);  //Bleu
    } else if (Launcher_PID && !Lift_PID && !Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(255, 75, 75);  // Rouge (pas bon a changer)
      Blink_MODE = true;
    } else if (Launcher_PID && !Lift_PID && !Carosse_Auto && Child) {
      leds[1].setRGB(255, 75, 75);  // Rouge (pas bon a changer)
    } else if (!Launcher_PID && !Lift_PID && !Carosse_Auto && !Child && !Blink_MODE) {
      leds[1].setRGB(255, 255, 255);
      Blink_MODE = true;  //blanc
    } else if (!Launcher_PID && !Lift_PID && !Carosse_Auto && Child) {
      leds[1].setRGB(255, 255, 255);  //blanc
    } else {
      leds[1].setRGB(0, 0, 0);
      Blink_MODE = false;
    }

    /**************************************************
    DEL Launcher RPM et Carosse nombre de ball
    **************************************************/
    if (!Launcher_On && !Blink_Launcher) {
      leds[2].setRGB(255, 0, 0);
      Blink_Launcher = true;
    } else if (Launcher_On && !Launcher_Stanby && RPM >= SET_Point_RPM[SET_RPM] && !Blink_Launcher) {
      Blink_Launcher = true;
      Couleur_DEL_x9(SET_RPM);
      leds[2].setRGB(Array_DEL[0], Array_DEL[1], Array_DEL[2]);
    } else if ((Launcher_On && !Launcher_Stanby && RPM < SET_Point_RPM[SET_RPM])) {
      Couleur_DEL_x9(SET_RPM);
      leds[2].setRGB(Array_DEL[0], Array_DEL[1], Array_DEL[2]);
    } else if (Launcher_On && Launcher_Stanby && !Blink_Launcher) {
      Blink_Launcher = true;
      Couleur_DEL_x9(ballCount);
      leds[2].setRGB(Array_DEL[0], Array_DEL[1], Array_DEL[2]);
    } else if (Launcher_On && Launcher_Stanby && Blink_Launcher) {
      leds[2].setRGB(100, 100, 100);
      Blink_Launcher = false;
    } else {
      leds[2].setRGB(0, 0, 0);
      Blink_Launcher = false;
    }
    FastLED.show();
    Refresh_Del = millis();
  }
}

/**************************************************
   Détection rising edge et debounce
**************************************************/
bool Bouton_1press(bool Current_Buton_state, bool Last_Buton_state) {
  static unsigned long minuterieDBNC = 0;
  static byte delaiDBNC = 1;
  if (Current_Buton_state && !Last_Buton_state && millis() - minuterieDBNC >= delaiDBNC) {
    return (true);
    minuterieDBNC = millis();
  } else {
    return (false);
  }
}


void Bouton() {
  bool ARROW_UP = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
  bool ARROW_LEFT = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
  bool ARROW_RIGHT = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
  bool ARROW_DOWN = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
  bool COLORS_UP = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
  bool COLORS_LEFT = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
  bool COLORS_RIGHT = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
  bool COLORS_DOWN = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
  bool L1 = CrcLib::ReadDigitalChannel(BUTTON::L1);
  bool R1 = CrcLib::ReadDigitalChannel(BUTTON::R1);
  bool START = CrcLib::ReadDigitalChannel(BUTTON::START);
  bool SELECT = CrcLib::ReadDigitalChannel(BUTTON::SELECT);
  bool LOGO = CrcLib::ReadDigitalChannel(BUTTON::LOGO);

  static bool Last_ARROW_UP;
  static bool Last_ARROW_LEFT;
  static bool Last_ARROW_RIGHT;
  static bool Last_ARROW_DOWN;
  static bool Last_COLORS_UP;
  static bool Last_COLORS_LEFT;
  static bool Last_COLORS_RIGHT;
  static bool Last_COLORS_DOWN;
  static bool Last_L1;
  static bool Last_R1;
  static bool Last_START;
  static bool Last_SELECT;
  static bool Last_LOGO;

  /**************************************************************
  Combinaison pour le changement des différents modes 
  ***************************************************************/
  if (START && Bouton_1press(ARROW_UP, Last_ARRW_UP)) {
    Launcher_PID = !Launcher_PID;
  } else if (START && Bouton_1press(ARROW_LEFT, Last_ARROW_LEFT)) {
    Carosse_Auto = !Carosse_Auto;
    //} else if (START && Bouton_1press(ARROW_RIGHT, Last_ARROW_RIGHT)) {// old
    //  Child = !Child;
  } else if (START && Bouton_1press(ARROW_DOWN, Last_ARROW_DOWN)) {
    Lift_PID = !Lift_PID;
  }
  /****************************************************************
  Child toggle new
  ****************************************************************/
  if (Bouton_1press(LOGO, Last_LOGO)) {
    Child = !Child;
  }

  /**************************************************************
  ARROWS 
  UP : RPM++
  LEFT : Launcher on/off
  RIGHT : Launcher stanby/ Spin up
  Down : Launcher RPM --
  ***************************************************************/
  else if (Bouton_1press(ARROW_UP, Last_ARROW_UP) && SET_RPM < 9) {
    SET_RPM++;
    //RPM++
  } else if (Bouton_1press(ARROW_LEFT, Last_ARROW_LEFT)) {
    //Launcher on/off (setpoint 0)
    Launcher_On = !Launcher_On;
  } else if (Bouton_1press(ARROW_RIGHT, Last_ARROW_RIGHT)) {
    //Launcher stanby (setpoint stanby or setpoint
    Launcher_Stanby = !Launcher_Stanby;
  } else if (Bouton_1press(ARROW_DOWN, Last_ARROW_DOWN) && SET_RPM > 1) {  // postion 0 réserver au Stanby
    //Launcher RPM--
    SET_RPM--;
  }

  /**************************************************************
  Fan
  ***************************************************************/
  if (Bouton_1press(COLORS_UP, Last_COLORS_UP)) {
    Fan = !Fan;
  }


  /**************************************************************
  Kirby 
  ***************************************************************/
  if (R1) {
    CrcLib::SetPwmOutput(CRC_PWM_10, -128);  // prend ball
  } else if (COLORS_DOWN) {
    CrcLib::SetPwmOutput(CRC_PWM_10, 127);  // drop ball
  } else {
    CrcLib::SetPwmOutput(CRC_PWM_10, 0);
  }




  /**************************************************************
  Activer/désactiver mode Serial Rpm sur manette
  ***************************************************************/
  if (Bouton_1press(SELECT, Last_SELECT)) {
    Serial_RPM = !Serial_RPM;
  }

  Last_ARROW_UP = ARROW_UP;
  Last_ARROW_LEFT = ARROW_LEFT;
  Last_ARROW_RIGHT = ARROW_RIGHT;
  Last_ARROW_DOWN = ARROW_DOWN;
  Last_COLORS_UP = COLORS_UP;
  Last_COLORS_LEFT = COLORS_LEFT;
  Last_COLORS_RIGHT = COLORS_RIGHT;
  Last_COLORS_DOWN = COLORS_DOWN;
  Last_L1 = L1;
  Last_R1 = R1;
  Last_START = START;
  Last_SELECT = SELECT;
  Last_LOGO = LOGO;
}

void setup() {
  CrcLib::Initialize();
  Serial.begin(115200);

  CrcLib::InitializePwmOutput(CRC_PWM_1);       // Drive Gauche
  CrcLib::InitializePwmOutput(CRC_PWM_2);       // Launcher
  CrcLib::InitializePwmOutput(CRC_PWM_3);       // Fan
  CrcLib::InitializePwmOutput(CRC_PWM_9);       // Lift
  CrcLib::InitializePwmOutput(CRC_PWM_10);      // Kirby
  CrcLib::InitializePwmOutput(CRC_PWM_11);      // strap
  CrcLib::InitializePwmOutput(CRC_PWM_12);      // Drive Droite
  CrcLib::SetDigitalPinMode(CRC_DIG_1, INPUT);  // limit switch Kirby
  CrcLib::SetDigitalPinMode(CRC_DIG_2, INPUT);  // limit switch Carrosse position 1
  CrcLib::SetDigitalPinMode(CRC_DIG_3, INPUT);  // limit switch Launcher

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);




  PID_Lift.setPoint(0);
  PID_Lift.setOutputRange(-80, 127);  // PWM range
  PID_Lift.setReverse(true);
  PID_Lift.setInterval(50);
  PID_Lift.setK(0.24, 0.48, 0.04);  // a modifier overshoot un peu beaucoup



  PID_Launcher.setPoint(0);
  PID_Launcher.setOutputRange(0, 128);  // PWM range
  PID_Launcher.setReverse(false);
  PID_Launcher.setInterval(75);  // 75 marche si possible vérifier 50 ou 100
  PID_Launcher.setK(0.08, 0.08, 0.0002);
}


void loop() {

  CrcLib::Update();
  DEL();
  Enco_Lift_V = Enco_Lift.read();
  Enco_Launcher_V = Enco_Launcher.read();


  if (Serial.available() > 0) {
    receivedCommand = Serial.readString();
    dataProcessing(receivedCommand);
  }

  /**************************************
   manette Déco 
   tout a 0 sauf Fan
  **************************************/
  if (!CrcLib::IsCommValid()) {
    CrcLib::MoveArcade(0, 0, CRC_PWM_1, CRC_PWM_12);
    CrcLib::SetPwmOutput(CRC_PWM_2, 0);
    CrcLib::SetPwmOutput(CRC_PWM_10, 0);
    CrcLib::SetPwmOutput(CRC_PWM_11, 0);


    /**************************************
    drop le lift doucement
    **************************************/
    if (Enco_Lift_V <= -5) {  // val lift de 0 a -2570
      Lift();
    } else {
      CrcLib::SetPwmOutput(CRC_PWM_9, 0);
      PID_Lift.stop();
    }

    PID_Launcher.stop();


    /**************************************
   manette Connecter
  **************************************/
  } else {

    if (Fan) {
      CrcLib::SetPwmOutput(CRC_PWM_3, 127);  //100%
    } else
      CrcLib::SetPwmOutput(CRC_PWM_3, 0);


    /***********************************************
    Arrete les PID si la batterie déconnecter 
    (évite probleme lorsque board est brancher usb) 
    ************************************************/
    if (CrcLib::GetBatteryVoltage(1.014) <= 10) {
      PID_Lift.stop();
      PID_Launcher.stop();
    }



    if (Child) {
      Child_1 = 50;
      Child_2 = -50;
    } else {
      Child_1 = 127;
      Child_2 = -128;
    }
    joy1_Y = map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y), -128, 127, Child_1, Child_2);  // map joy a valeur PWM (a changer de place)
    joy1_X = map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X), -128, 127, Child_1, Child_2);  // map joy a valeur PWM (a changer de place)
    CrcLib::MoveArcade(joy1_Y, joy1_X, CRC_PWM_1, CRC_PWM_12);

    /***********************************************
    Fonction des composants principaux
    ************************************************/
    Bouton();
    Lift();
    Launcher();
    Carosse();
  }
}