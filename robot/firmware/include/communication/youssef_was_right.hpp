// so, youssef was right about the spi situation (who knew?)...

#include <SPI.h>

constexpr size_t NUM_ENCOS = 8;
constexpr size_t DATAFRAME_BUFFER_LEN = (sizeof(int32_t) * NUM_ENCOS);

using dataframe_t = int32_t[NUM_ENCOS];
union dataframe_softcast
{
    dataframe_t df;
    uint8_t raw[DATAFRAME_BUFFER_LEN];
};

SPISettings ctrlSPI(1000000, MSBFIRST, SPI_MODE0);

#ifdef ARDUINO_SAM_DUE
/**
 * master code
 */

#include <Encoder.h>

#define CS_SPI_PIN 10

void spi_setup()
{
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    SPI.begin();
}

void transmit_df(dataframe_t df)
{
    // dataframe_softcast dfs;
    // dfs.df=df;
    
    SPI.beginTransaction(ctrlSPI);
    digitalWrite(10, LOW);
    auto raw = reinterpret_cast<byte*>(df);
    for (size_t i = 0; i < DATAFRAME_BUFFER_LEN; i++) {
        if (i%4==0) Serial.print(" ");
        Serial.print(raw[i], HEX);
    }
    Serial.println();
    SPI.transfer(df, DATAFRAME_BUFFER_LEN);
    
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
// byte buffer[DATAFRAME_BUFFER_LEN];
dataframe_softcast dfs;

void spi_setup()
{
    // pinMode(CS_SPI_PIN, OUTPUT);
    pinMode(MISO, OUTPUT);  //Configure MISO en sortie (pour envoyer des données au contrôleur)
    // pinMode(MOSI, OUTPUT);
    // digitalWrite(CS_SPI_PIN, HIGH);
    SPCR |= _BV(SPE);      //Configure l'interface SPI comme étant un périphérique
    // // SPI.begin();
    SPI.attachInterrupt(); // Permet les interruptions sur la réception de données SPI


    // Enable SPI in Slave mode
    //SPCR = (1 << SPE);  // Enable SPI, Slave mode (MSTR = 0 by default)

    // Enable SPI Transfer Complete Interrupt
    // SPCR |= (1 << SPIE);
    // sei();  // Enable global interrupts


    // SPI.begin();
    // // SPCR |= (1 << SPIE);
    // SPI.attachInterrupt(); // Permet les interruptions sur la réception de données SPI
}

ISR(SPI_STC_vect)
{
    byte receivedByte = SPDR;

    if (received) {
        return;
    }

    if (index < DATAFRAME_BUFFER_LEN) {
        dfs.raw[index++] = receivedByte;
    }

    // Mark reception complete when buffer is full
    if (index >= DATAFRAME_BUFFER_LEN) {
        received = true;
        index = 0;  // Reset index for next message
    }
}

void update_df(dataframe_t &df)
{
    // copy the bytes when the data is ready
    if (received)
    {
        // dataframe_softcast dfs;
        // memcpy(df, buffer, DATAFRAME_BUFFER_LEN);
        for (size_t i =0; i < NUM_ENCOS; i++ ) {
            df[i] = dfs.df[i];
        }
        received = false;
    }
    // if (index != 0 && digitalRead(CS_SPI_PIN) == HIGH) {
    //     index = 0;
    // }
}

#endif


// dataframe_t update_df()
// {
//     dataframe_softcast dfs;
    
//     // copy the bytes when the data is ready
//     if (received)
//     {
//         dfs.raw = buffer;
//         memcpy(dfs.raw, buffer, )
//         // for (size_t i = 0; i < NUM_ENCOS; i++) {
//         //     df[i] = dfs.df[i];
//         // }
//         received = false;
//     }
//     // if (index != 0 && digitalRead(CS_SPI_PIN) == HIGH) {
//     //     index = 0;
//     // }
//     return dfs.df;
// }

// #endif