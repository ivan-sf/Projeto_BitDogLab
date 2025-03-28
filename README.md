<div align="center">
    <img src="https://github.com/user-attachments/assets/337c76d6-a1cc-414f-bd90-866fe408b844" alt="V-CC_Color_Basica_TextoAzul" style="width:50%;"/>
</div>

# Biblioteca para Display OLED SSD1306 utilizando PICO-SDK

Esta biblioteca foi desenvolvida para um Display OLED SSD1306, em RP2040, via protocolo de comunicação I2C, utilizando o PICO-SDK. Para isso, foi utilizada a plataforma de ensino baseada em tecnologia aberta BitDogLab. Com suporte a formas geométricas e textos, ela permite criar interfaces visuais eficientes para projetos embarcados.

<div align = "center">
    <img src="https://github.com/user-attachments/assets/17d56731-aa0e-4e26-ae65-f6f476a55f2b" alt="BDL1" style="width:25%;"/> <img src="https://github.com/user-attachments/assets/bcf95a15-ca86-45f4-aeb4-2b736a7ed5b3" alt="BDL2" style="width:25%;">
</div>

---

## Recursos

- Inicialização e configuração do display SSD1306;
- Controle total de pixels (acender/apagar); 
- Desenho de formas geométricas (linhas, retângulos, triângulos, círculos e arcos);  
- Suporte a fontes customizadas e exibição de texto.    

---

## Dependências

Antes de usar a biblioteca, certifique-se de ter o SDK do Raspberry Pi Pico instalado. O projeto depende das seguintes bibliotecas padrão:

- `hardware/i2c.h` (para comunicação I2C)
- `stdint.h`, `stdlib.h`, `string.h`

---

## Como Utilizar a Biblioteca

### Configuração

1. **Inclua as bibliotecas no seu projeto:**
   ```c
   #include "SSD1306.h"
   #include "Fonts.h"
   ```

2. **Declare a estrutura de renderização e o buffer:**
   ```c
   struct RenderArea FrameArea = {
    .StartColumn = 0,
    .EndColumn = SSD1306_WIDTH - 1,
    .StartPage = 0,
    .EndPage = SSD1306_NumberOfPages - 1
   };

   uint8_t ssd[SSD1306_BufferLength];
   ```

3. **Na main, utilize as funções de cálculo de área de renderização e de inicialização:**
   ```c
   Calculate_RenderArea_BufferLength(&FrameArea);

   SSD1306_Init();
   ```

### Funções

O arquivo ``` SSD1306.h ``` contém instruções acerca da utilização das funções e seus respectivos parâmetros.

1. **Figuras geométricas:**
    ```c
    void SSD1306_DrawLine(uint8_t *ssd, int X0, int Y0, int X1, int Y1, SSD1306_COLOR_t color);
    void SSD1306_DrawRectangle(uint8_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color);
    void SSD1306_DrawFilledRectangle(uint8_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color);
    void SSD1306_DrawTriangle(uint8_t *ssd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
    void SSD1306_DrawFilledTriangle(uint8_t *ssd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
    void SSD1306_DrawCircle(uint8_t *ssd, int16_t x_center, int16_t y_center, int16_t radius, SSD1306_COLOR_t color);
    void SSD1306_DrawFilledCircle(int8_t *ssd, int16_t x_center, int16_t y_center, int16_t radius, SSD1306_COLOR_t color);
    void SSD1306_DrawArc(uint8_t *ssd, uint16_t x_center, uint16_t y_center, uint16_t radius, uint16_t start_angle, uint16_t end_angle, SSD1306_COLOR_t color);
    ```

2. **Textos:**
   ```c
   void SSD1306_SetChar(uint8_t *ssd, int16_t x, int16_t y, uint8_t character, FontDef_t* Font, SSD1306_COLOR_t color);
   void SSD1306_SetString(uint8_t *ssd, int16_t x, int16_t y, const char *str, FontDef_t* Font, SSD1306_COLOR_t color);
   void SSD1306_SetStringWrapped(uint8_t *ssd, int16_t x, int16_t y, const char *str, FontDef_t* Font, SSD1306_COLOR_t color);
   ```
3. **Atualização:**
   ```c
   void SSD1306_Render(uint8_t *ssd, struct RenderArea *area);
   void SSD1306_Clear(uint8_t *ssd, struct RenderArea *area);
   ```

---
   
## Exemplo
Veja o arquivo "SSD1306_Exemplo.c", no qual há testes para cada função separadamente e, além disso, uma sequência de uso de funções em conjunto, tanto para as formas geométricas,
quanto para as de exibição de texto. Note que o buffer **ssd** é utilizado em todas as funções (exceto **SSD1306_Init**) e a estrutura de renderização **FrameArea** apenas nas funções de controle de atualização do display
(**SSD1306_Render** e **SSD1306_Clear**).

---

## Considerações Finais
Embora essa biblioteca considera o display presente na BitDogLab por padrão, pode-se facilmente adaptá-la para um display de outro tamanho ou que tenha outro endereço I2C, como o 0x7A. Para isso, modifique
os seguintes parâmetros:
    ```
    #define SSD1306_I2C_ADDRESS 0x3C
    ```
por
    ```
    #define SSD1306_I2C_ADDRES 0x7A
    ```
e
    ```
    #define SSD1306_HEIGHT 64
    ```
por
    ```
    #define SSD1306_HEIGHT 32
    ```.
