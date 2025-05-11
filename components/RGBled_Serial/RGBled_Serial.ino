#define red 5
#define green 18
#define blue 19

String inputString = "";

void setup() {
  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.println("Enter RGB values (e.g., 255,0,255) or 'off':");
}

void loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      inputString.trim();  // Clean up any whitespace or carriage returns

      if (inputString.equalsIgnoreCase("off")) {
        setColor(0, 0, 0);
        Serial.println("LED turned off.");
      } else {
        int r, g, b;
        if (parseRGB(inputString, r, g, b)) {
          setColor(r, g, b);
          Serial.print("Color set to: ");
          Serial.print(r); Serial.print(", ");
          Serial.print(g); Serial.print(", ");
          Serial.println(b);
        } else {
          Serial.println("Invalid input. Use format: R,G,B (0-255)");
        }
      }

      inputString = ""; // Clear the string for next input
    } else {
      inputString += inChar;
    }
  }
}

void setColor(int r, int g, int b) {
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}

bool parseRGB(String str, int &r, int &g, int &b) {
  int firstComma = str.indexOf(',');
  int secondComma = str.indexOf(',', firstComma + 1);

  if (firstComma == -1 || secondComma == -1) return false;

  String rStr = str.substring(0, firstComma);
  String gStr = str.substring(firstComma + 1, secondComma);
  String bStr = str.substring(secondComma + 1);

  r = rStr.toInt();
  g = gStr.toInt();
  b = bStr.toInt();

  if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) return false;

  return true;
}

