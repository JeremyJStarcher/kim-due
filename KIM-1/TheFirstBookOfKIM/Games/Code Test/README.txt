CODE TEST
BY STAN OCKERS


  DESCRIPTION -
     THIS PROGRAM REQUIRES THAT A SPEAKER BE HOOKED
  TO PA0 AS IN FIGURE 5.1 OF THE KIM MANUAL.  WHEN STARTED
  AT 0200, THE PROGRAM WILL SEND 5 LETTER CODE GROUPS,
  (INTERNATIONAL MORSE), OVER THE SPEAKER.  THE CODE GROUPS
  WILL CONSIST OF RANDOM CHARACTERS INCLUDING A-Z, 0-9, A
  PERIOD, COMMA, QUESTION MARK AND EQUAL SIGN.  AFTER THIS
  TRANSMISSION, YOUR RECEPTION CAN BE CHECKED BECAUSE THE
  GROUPS SENT WILL BE SHOWN ON THE DISPLAY.  PRESSING ANY
  KEY WILL CAUSE THE NEXT GROUP TO BE DISPLAYED.  LIMITATIONS
  IMPOSED BY THE 7 SEGMENT DISPLAYS MAKE SOME CHARACTERS
  PRETTY STRANGE AND THERE IS SOME REDUNDANCY; BUT BY SLOWING
  THE TRANSMISSION YOU SHOULD BE ABLE TO FIGURE OUT WHAT
  EACH CHARACTER IS.

CHANGES-
     THE PROGRAM IS INITIALLY SET UP TO SEND AND DISPLAY
5 GROUPS OF 5 CHARACTERS EACH.  THEY ARE SENT AT A RATE
OF ABOUT 16 GROUPS PER MINUTE.  ALL OF THIS CAN OF
COURSE BE CHANGED.
- THE NUMBER OF CHARACTERS TO BE SENT (IN HEX) PLUS ONE
  SHOULD BE STORED IN 0233, (INITIALLY 1A).
- THE NUMBER OF GROUPS TO BE DISPLAYED AFTER TRANSMISSION
  SHOULD BE STORED IN 02E0  (INITIALLY 05).
- THE SPEED OF TRANSMISSION IS DETERMINED BY THE CONTENTS
  OF 02E3.  A HEX 33 GIVES ABOUT 16 GROUPS/MINUTE, A
  66 GIVES 8 WPM.
- THE TONE CAN BE VARIED BY THE CONTENTS OF 00E4.
- A MAXIMUM OF ONE PAGE OF CHARACTERS CAN BE SENT
  STORED IN A BLOCK POINTED TO BY 02E1 AND 02E2.
- FOR A DESCRIPTION OF HOW THE CHARACTERS ARE STORED,
  SEE OCT. '76 BYTE, PAGE 36.
- A PORTION OF THE CHARACTER SET, (SAY ONLY LETTERS),
  CAN BE SELECTED BY ADJUSTING THE BYTE AT 0222.

CORRECTIONS:

CODE TEST, page 58:  addresses 02CE through 02DA should be changed to:
       D1 65 D4 65 D5 85 D0 A2 04 B5 D0 95 D1

CODE TEST, Page 58: zero page locations in random number routine
duplicate others, change E's to D's. Send for re-write (other errors)

 CODE TEST, p.58: change 02CE thru 02DA to:
       D1 65 D4 65 D5 85 D0 A2 04 B5 D0 95 D1
