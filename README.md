# 🛠️ Controle de Joystick e Display OLED com RP2040

## 📜 Descrição do Projeto  

Este projeto foi desenvolvido para consolidar os conceitos relacionados ao uso de modulação por largura de pulso (PWM) no microcontrolador RP2040. Utilizando a placa de desenvolvimento Raspberry Pi Pico W, a aplicação funcional controla o ângulo de um servomotor padrão, simulando seu comportamento no ambiente Wokwi. O projeto também explora a integração do Pico SDK com o VS Code e a ferramenta BitDogLab para análise de comportamento de LEDs.
## 🎯 Objetivos  

- **Compreender** o funcionamento do módulo PWM no RP2040.
- **Aplicar PWM** para controle de um servomotor.
- **Simular** o comportamento do servomotor no ambiente Wokwi.
- **Integrar** hardware e software em uma aplicação funcional.
- **Analisar** o comportamento de um LED RGB utilizando a ferramenta BitDogLab.  

---

## 🚀 Componentes Utilizados  

- **Microcontrolador Raspberry Pi Pico W**
- **Servomotor – motor micro servo padrão** (simulado no Wokwi)
- **LED RGB** (conectado à GPIO 12)
- **Ambiente de desenvolvimento VS Code**
- **Simulador Wokwi**
- **Ferramenta Educacional BitDogLab**

---

## ⚡ Funcionalidades  

**Controle de Servomotor:**
- Ajuste do ângulo do servomotor para 0°, 90° e 180° utilizando PWM.
- Movimentação suave do braço do servomotor entre 0° e 180°.

**LED RGB:**
- Análise do comportamento do LED RGB utilizando a ferramenta BitDogLab.

---

## 🛠️ Dependências

- **CMake** - Gerador de builds 
- **Ninja** - Ferramenta de build rápida 
- **Python 3** - Interpretador Python

#### Download das Dependências caso não possua:
1. [**Cmake**](https://cmake.org/download/)
2. [**Ninja**](https://github.com/ninja-build/ninja/releases)
3. [**Python3**](https://www.python.org/downloads/)

## 🔧 Instalação
###   Adicionar Ninja, CMake e Python ao Path do Sistema  

Para que os comandos **Ninja**, **CMake** e **Python** sejam reconhecidos em qualquer terminal, siga estas etapas:  

1. **Abra as Configurações do Sistema:**  
   - Clique com o botão direito no botão **Iniciar** e selecione **Configurações**.  
   - Vá em **Sistema → Sobre → Configurações Avançadas do Sistema** (no lado direito).  

2. **Acesse as Variáveis de Ambiente:**  
   - Na aba **Avançado**, clique em **Variáveis de Ambiente**.  
   - Na seção **Variáveis do Sistema**, localize a variável `Path` e clique em **Editar**.  

3. **Adicione os Caminhos dos Programas:**  

   - **Ninja:**  
     Clique em **Novo** e insira o caminho onde você extraiu o `ninja.exe`. Exemplo:  
     ```
     C:\Users\SeuUsuario\Downloads\ninja-win
     ```  

   - **CMake:**  
     Clique em **Novo** e adicione o diretório `bin` do CMake. Exemplo:  
     ```
     C:\Program Files\CMake\bin
     ```  

   - **Python:**  
     Clique em **Novo** e adicione o diretório onde o Python foi instalado. Exemplo:  
     ```
     C:\Python39\
     ```  

4. **Salve as Alterações:**  
   - Clique em **OK** em todas as janelas para salvar as configurações.  

5. **Verifique as Instalações:**  

   Abra o terminal e digite os seguintes comandos para garantir que estão configurados corretamente:  

   ```bash
   ninja --version
   cmake --version
   python --version


## 🖱️ Como executar

### Abra o terminal e siga os passos abaixo:


1. Clone este repositório:

   ```bash
   git clone https://github.com/EbtechRep/ADC.git
   ```
2. Importe o projeto pela extensão do **Rasquery Pi Pico Project**

3. Acesse o diretório do projeto:

   ```bash
   cd main.c
   ```

3. Instale a pasta **build**

   ```bash
    mkdir build
   ```

4. Acesse o diretório **build**

   ```bash
   cd build
   ```
5. Compile o projeto com CMake e Ninja:
   ```bash
   cmake -G "Ninja" ..
   ```   
6. Execute a compilação:
   ```bash
   ninja
   ```   
## 💻 Video demonstrativo: 
**https://youtu.be/JX5RWcU1yeE**
 








