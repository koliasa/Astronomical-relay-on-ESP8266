## Астрономічне Реле на ESP8266

Цей код для платформи ESP8266 дозволяє керувати реле в залежності від астрономічних даних, а також за ручним графіком. Реле може вмикатись і вимикатись автоматично згідно годин сходу та заходу сонця, а також відповідно до вручну заданих годин вмикання та вимикання.

## Перелік необхідного обладнання та компонентів

- ESP8266 Development Board (NodeMCU або подібний модуль)
- Реле (2 штуки) або 1 двоканальне реле
- Кнопки (2 штуки)
- Резистори (2 штуки, приблизно 10 кОм)
- Дріт та перехідники
- USB-кабель
- Джерело живлення (USB-адаптер або батарея)
- ПК або ноутбук

### Параметри

- `ssid` та `password`: Налаштування для підключення до Wi-Fi мережі.
- `ntpServer`, `gmtOffset`, та `daylightOffset`: Налаштування для синхронізації з NTP сервером для отримання точного часу.
- `relayPin1`, `relayPin2`: Піни для підключення реле.
- `buttonPin1`, `buttonPin2`: Піни для підключення кнопок ручного керування.
- `apiKey`, `city`, `country`: Параметри для отримання астрономічних даних з OpenWeatherMap.

### Функціонал

1. **Автоматичний режим астрономічного керування**: Реле 1 буде вмикатись згідно годин сходу та заходу сонця. Реле 2 буде вмикатись після заходу сонця та вимикатись перед сходом сонця. Це забезпечує автоматичне керування освітленням у відповідності до доби.

2. **Ручний режим керування**: За допомогою кнопок на платі ESP8266 можна вручну вмикати та вимикати кожне реле незалежно. Це дозволяє вам контролювати освітлення вручну за необхідності.

3. **Автоматичне зсування годин**: Враховуючи пору року, реле буде вмикатись та вимикатись на пізніших чи раніших годинах, щоб відповідати періодам сходу та заходу сонця. Це допомагає забезпечити оптимальне освітлення в будь-яку пору року.

### Використання

1. Підключіть ESP8266 до Wi-Fi мережі за допомогою налаштувань `ssid` та `password`.

2. Вкажіть параметри для отримання астрономічних даних з OpenWeatherMap, такі як `apiKey`, `city`, `country`.

3. Підключіть реле та кнопки до відповідних пінів на ESP8266.

4. Завантажте код на ESP8266 та спостерігайте, як реле керуються автоматично згідно астрономічних даних та ручних налаштувань.

Цей код дозволяє Вам зручно керувати освітленням на основі астрономічних параметрів, забезпечуючи ефективне та енергозберігаюче освітлення в будь-яку пору доби та року.
