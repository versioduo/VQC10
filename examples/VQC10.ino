#include <Adafruit_TinyUSB.h>
#include <VQC10.h>

static VQC10<> LED({
  .digits{A0, A1, A2, A3},
  .columns{A4, A5, 23, 2, 3},
  .rows{5, 7, 9, 10, 11, 12, 13},
});

// https://de.wikipedia.org/wiki/Politisches_System_der_DDR
static const char text[] = "Das politische System der DDR war eine Diktatur ohne eine tatsaechliche "
                           "Gewaltenteilung. Sie vereinigte die Eigenschaften des realen Sozialismus "
                           "mit den Prinzipien des so genannten Demokratischen Zentralismus. Die "
                           "politische Macht war nicht auf verschiedene Traeger verteilt. Unter "
                           "Widerspruch zu Artikel 5 Satz 3 der Verfassung der DDR ging sie stattdessen "
                           "fuer die gesamte Zeit ihrer Existenz von dem umfassend und unkontrolliert "
                           "herrschenden Fuehrungs- und Herrschaftszentrum der DDR aus, dem Politbuero des "
                           "Zentralkomitees der Sozialistischen Einheitspartei Deutschlands (SED), das "
                           "ebendieser marxistisch-leninistischen Partei vorstand, welche ihrerseits "
                           "(durch Artikel 1 der Verfassung der DDR) einen Alleinfuehrungsanspruch fuer alle "
                           "Bereiche der DDR erhob. "
                           "Die Deutsche Demokratische Republik war im Selbstverstaendnis ein sozialistischer "
                           "Staat und verwirklichte die Grundprinzipien einer Volksrepublik. Da die "
                           "Regierungsform durch die Herrschaft einer Partei, der so genannten Staatspartei, "
                           "gepraegt wurde, spricht man bei der DDR auch von einer Parteidiktatur.    ";

void setup() {
  Serial.begin(9600);
  LED.begin();
}

void loop() {
  static unsigned long usec{};
  static uint16_t count{};

  if ((unsigned long)(micros() - usec) > 150 * 1000) {
    usec = micros();

    LED.show(0, text[count + 0]);
    LED.show(1, text[count + 1]);
    LED.show(2, text[count + 2]);
    LED.show(3, text[count + 3]);

    count++;
    if (count + 3 == sizeof(text))
      count = 0;
  }

  LED.loop();
}
