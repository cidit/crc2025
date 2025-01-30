// so, youssef was right about the spi situation (who knew?)...

#include <SPI.h>

constexpr size_t NUM_ENCOS = 8;
constexpr size_t DATAFRAME_BUFFER_LEN = (sizeof(int32_t) * NUM_ENCOS);

using dataframe_t = int32_t[NUM_ENCOS];
SPISettings ctrlSPI(1000000, MSBFIRST, SPI_MODE0);

#ifdef ARDUINO_SAM_DUE
/**
 * master code
 */

#include <Encoder.h>

#define CS_SPI_PIN 10

void spi_setup()
{
    SPI.begin();
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
}

void transmit_df(dataframe_t df)
{
    SPI.beginTransaction(ctrlSPI);
    digitalWrite(10, LOW);

    auto raw = reinterpret_cast<byte*>(df);
    for (size_t i = 0; i < DATAFRAME_BUFFER_LEN; i++) {
        SPI.transfer(raw[i]);
    }

    digitalWrite(10, HIGH);
    SPI.endTransaction();
}

#else
/**
 * slave code
 */

#define CS_SPI_PIN 53

// typedef struct mystruct {
//   // uint32_t EncoSwerveR_A;
//   uint32_t EncoSwerveR_B;
// };

// typedef union patate {
//   mystruct spiMsg;
//   byte table[sizeof(mystruct)];
// };
// patate msg;

volatile bool received = false; // set to false if ready for next message

byte index = 0;
byte buffer[DATAFRAME_BUFFER_LEN];

void spi_setup()
{
    // pinMode(CS_SPI_PIN, OUTPUT);
    // // pinMode(MISO, INPUT);  //Configure MISO en sortie (pour envoyer des données au contrôleur)
    // // pinMode(MOSI, OUTPUT);
    // digitalWrite(CS_SPI_PIN, HIGH);
    // // SPCR |= _BV(MSTR);      //Configure l'interface SPI comme étant un périphérique
    // SPI.attachInterrupt(); // Permet les interruptions sur la réception de données SPI


    // Enable SPI in Slave mode
    SPCR = (1 << SPE);  // Enable SPI, Slave mode (MSTR = 0 by default)

    // Enable SPI Transfer Complete Interrupt
    SPCR |= (1 << SPIE);
    sei();  // Enable global interrupts
}

ISR(SPI_STC_vect)
{
    byte receivedByte = SPDR;
    buffer[index++] = receivedByte;
    if (index == DATAFRAME_BUFFER_LEN)
    {
        index = 0;
        received = true;
    }
}

void update_df(dataframe_t &df)
{
    // copy the bytes when the data is ready
    if (received)
    {
        auto raw_df = reinterpret_cast<byte*>(df);
        // memcpy(df, buffer, DATAFRAME_BUFFER_LEN);
        for (size_t i =0; i < DATAFRAME_BUFFER_LEN; i++ ) {
            raw_df[i] = buffer[i];
        }
        received = false;
    }
    // if (index != 0 && digitalRead(CS_SPI_PIN) == HIGH) {
    //     index = 0;
    // }
}

#endif