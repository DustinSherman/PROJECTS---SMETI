# KONZEPT

## SMETI

SMETI steht für sythesized messages for extraterestrial intelligence life. Dieser Prozess sieht vor eine Nachricht über Klangsynthese zu codieren so dass sie im Idealfall unabhängig von Kultur und Sprache verstanden werden kann Der Sender stellt dafür am Gerät selber den Inhalt der Nachricht zusammen um anschließend als Klangsynthese wieder gegeben werden. Der erste Wert bestimmt dabei ob und welche Art von Begrüßungsnachricht vorn an gestellt werden soll, der zweite Wert ob es sich um eine Frage oder Aussage handelt und auf wen sich diese bezieht. Mit dem dritten Wert lassen sich quantitive Aussagen machen über Distanz, Zeit und Anzahl. Der vierte Wert kann Aussagen über interstellare Objekte machen und orientiert sich dabei an den Objekten in unserem Sonnensystem (Jupiter, Venus, Mars, Erde, Saturn (Ringe), Sonne).

## Inhalte

- **WERT 1** - Intro
	- 0 - Keine Einführung
	- 1 - Einführung Dezimalsystem
	- 2 - Einführung Dezimalsystem - Primzahlenfolge (1 - 17)
	- 3 - Einführung Dezimalsystem - Quadratzahlen (1 - 128)
	- 4 - Einführung Dezimalsystem - Zahl Pi (3,1415926)
	- 5 - Einführung Dezimalsystem - Fibonacci-Folge (1, 1, 2, 3, 5, 8, 13, 21)<br>
Mathematische Formeln und Folgen werden als Einführung der Nachricht vorn an gestellt um zu verdeutlichen dass es sich hierbei nicht um ein natürliches Phänomen handelt. Um diese Mathematik zu verstehen wird als gemeinsame Basis das menschliche Dezimal-System verwendet. Für jede Dezimalstelle wird hierfür ein Sinuston mit einer unterschiedlichen Frequenz abgespielt. 1er-Stellen werden mit 440Hz, 10er-Stellen mit 400Hz, 100er-Stellen mit 360Hz, 1000er-Stellen mit 320Hz, 10000er-Stellen mit 280Hz und 100000er-Stellen mit 240Hz. Die Einführung in das Dezimal-System sieht somit wie folgt aus: (0 = Pause, I = 1er(440Hz), II = 10er(400Hz) usw.) I 0 I I I I I I I I I I I II 0 II II II II II II II II II II III 0 III III III III III III III III III III IIII 0 IIII IIII ...<br>
*Einfacher Sinuston 440 Hz (Kammerton)*
- **WERT 2** - Bezug / Aussage
	- 0 - Kein Bezug
	- 1 - Ihr
	- 2 - Wir
	- 3 - Frage
	- 4 - Aussage
	- 5 - Ihr / Frage
	- 6 - Ihr / Aussage
	- 7 - Wir / Frage
	- 8 - Wir / Aussage<br>Dieser Wert bestimmt worauf sich die Nachricht bezieht und ob es sich um eine Frage oder eine Aussage handelt. Jeder Parameter kann auch alleine festgelegt werden.<br>
*Höher bzw. tiefer werdender Sinuston für Frage bzw. Aussage, welcher lauter bzw. leiser wird entsprechend dem Bezug 'Ihr' bzw. 'Wir'. Werden Form und Bezug gewählt werden die entsprechenden Töne gleichzeitig abgespielt.*
- **WERT 3** - Negativ / Positiv
	- 0 - Kein wertende Aussage
	- 1-4 - Sehr negativ bis Neutral Negativ
	- 5 - Neutral
	- 6-9 - Neutral Positiv bis Sehr positiv <br>Man kann mit Hilfe dieses Parameters bestimmen ob eine Aussage oder Frage eine negative oder positive Eigenschaft beifügen. Zu dem lassen sich damit Fragen beantworten sofern dies vom Fragekontext möglich ist. Die einstellbare Skala geht dabei von 1 (sehr negativ) über 4/5 (neutral) bis hin zu 9 (sehr positiv).<br>*Der erzeugte Ton ist eine Mischung aus einer Dur-Sinuswelle für postive Aspekte und einem Moll-Dreieckwelle für negative Aspekte die jeweils in ihrer Tonhöhe angepasst werden.*
- **WERT 4** 
 	- 0 - Keine quantitative Aussage
	- 1 - Distanz
	- 2 - Zeit
	- 3 - Anzahl<br>Hierüber lassen sich quantitive Aussagen über die Einheiten Zeit, Distanz und Anzahl machen. Man legt dabei zuerst fest was man darstellen möchte und im Anschluss die zu sendende Zahl.<br>
*Quantität wird über einen einfach Sinuston dargestellt (siehe Einführung). Distanz wird mittels eines Noise-Generators der im Panorama von rechts nach links wandert dargestellt. Zeit ist ein regelmäßiges, gleichmäßiges Ticken. Eine Anzahl wird entsprechend mit einem einfachen Sinuston dargestellt.*
- **WERT 5** - Interstellare Objekte<br>
	- 0 - Kein interstellares Objekt
	- 1 - Gasplanet (Jupiter)
	- 2 - Gesteinsplanet (Venus)

## Nachricht

Wurde eine Einführungsnachricht ausgewählt wird als erstes diese alleine abgespielt. Sobald diese fertig abgespielt ist wird der Bezug/Aussage-Wert wiedergegeben und zu dem wird zeitgleich der Ton des interstellaren Objekts im Hintergrund eingefadet. Anschließend wird der neagtive/positiv Wert abgespielt und ganz zum Schluss schließlich der quantitive Wert. Die Nachricht wird dabei so lange wiederholt bis der Encoder wieder gedrückt wird.