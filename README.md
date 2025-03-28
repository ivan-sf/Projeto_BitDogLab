<div align="center">
    <img src="https://github.com/user-attachments/assets/337c76d6-a1cc-414f-bd90-866fe408b844" alt="V-CC_Color_Basica_TextoAzul" style="width:50%;"/>
</div>

# Gravador e Analisador de Áudio - Projeto BitDogLab


Este projeto implementa uma interface gráfica utilizando um display OLED SSD1306 para a gravação e análise de áudio. Ele permite capturar áudio com um microfone, exibir a forma de onda no domínio do tempo e visualizar o espectro de frequência por meio de uma Transformada Rápida de Fourier (FFT). A navegação entre as opções é feita através de um joystick, e o projeto foi desenvolvido para a plataforma de ensino baseada em tecnologia aberta BitDogLab, que tem como microcontrolador o Raspberry Pi Pico W, RP2040.


<div align = "center">
    <img src="https://github.com/user-attachments/assets/17d56731-aa0e-4e26-ae65-f6f476a55f2b" alt="BDL1" style="width:25%;"/> <img src="https://github.com/user-attachments/assets/bcf95a15-ca86-45f4-aeb4-2b736a7ed5b3" alt="BDL2" style="width:25%;">
</div>

---

## Recursos

- **Gravação de Áudio:** Captura de áudio utilizando microfone.
- **Visualização no Domínio do Tempo:** Plotagem da forma de onda do áudio.
- **Visualização no Domínio da Frequência:** Análise espectral do áudio utilizando FFT.
- **Interface Interativa:** Navegação pelas telas (Gravar, Tempo, Frequência) por meio de um joystick.

---

<div align="center">
    <img src="https://github.com/user-attachments/assets/b9293ab2-ef0b-46b4-9065-545c11e2caf5" alt="menu" style="width:25%;"/> <img src="https://github.com/user-attachments/assets/e8e99df6-fff0-4221-abbf-19126dc5adb6" alt="tempo" style="width:25%;"/> <img src="https://github.com/user-attachments/assets/4d2f8256-0ab8-4bdd-bf0b-84c8201eb049" alt="fft" style="width:25%;"/>
</div>

## Componentes Utilizados

- **Display**: Exibição do Menu ([SSD1306](https://github.com/ivan-sf/SSD1306-Library-PICO-SDK))

- **Joystick**: Interação Com o Menu ([Joystick](https://github.com/AntonioJunior2222/BitDogLab-Joystick-Matriz_Led-VIRTUS_CC))

- **Microfone**: Gravação do áudio ([Microfone](https://github.com/smalljooj/bitdoglab_mic_VIRTUS_CC))


## Conexões e Configurações

O projeto utiliza a interface I2C para comunicação com o display OLED, conforme definido no código:
- **I2C Port:** `i2c1`
- **SDA:** GPIO 14
- **SCL:** GPIO 15
- **ADC** GPIO 28 (Microfone), GPIO 22 GPIO 26 GPIO 27 (Joystick)

