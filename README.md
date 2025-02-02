# tarefa-27jan
tarefa do dia 27 de janeiro.

Atividade avaliativa proposta pelo Cepedi como parte dos critérios de avaliação do curso de capacitação em software e sistemas embarcados (Embarcatech).

# Projeto: Controle de LEDs e Botões com Interrupções no RP2040

## 📚 Sumário
- [Proposta de Projeto](#-Proposta-de-Projeto)  
- [Objetivos](#-objetivos)  
- [Componentes Utilizados](#%EF%B8%8F-componentes-utilizados)  
- [Funcionalidades Implementadas](#%EF%B8%8F-funcionalidades-implementadas)  
- [Requisitos Técnicos](#-requisitos-técnicos)  
- [Como Executar](#-como-executar)  
- [Estrutura do Repositório](#-estrutura-do-repositório)  
- [Demonstração](#-demonstração)  
- [Contribuições](#-contribuições)  
- [Licença](#-licença)  
- [Contato](#-contato)   

## 📋 Proposta de Projeto  
Este projeto tem como objetivo consolidar os conceitos relacionados ao uso de interrupções em microcontroladores com a placa BitDogLab. O desenvolvimento propõe o controle de LEDs (comuns e endereçáveis) e a utilização de botões com tratamento de bouncing via software.  

## 🎯 Objetivos
- Compreender o funcionamento e aplicação de interrupções no microcontrolador RP2040.
- Implementar debouncing para botões através de software.
- Manipular LEDs comuns e LEDs WS2812 endereçáveis.
- Utilizar resistores de pull-up internos nos botões.
- Desenvolver uma aplicação funcional combinando hardware e software.  

## 🛠️ Componentes Utilizados
- **Matriz 5x5 de LEDs WS2812:** Conectada à GPIO 7.  
- **LED RGB:** Pinos conectados às GPIOs 11, 12 e 13.  
- **Botão A:** Conectado à GPIO 5.  
- **Botão B:** Conectado à GPIO 6.  

## ⚙️ Funcionalidades Implementadas  
1. O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.  
2. O botão A incrementa o número exibido na matriz de LEDs a cada pressão.  
3. O botão B decrementa o número exibido na matriz de LEDs a cada pressão.  
4. Os LEDs WS2812 exibem números de 0 a 9 com formato digital fixo ou criativo.  

## 📝 Requisitos Técnicos
- **Interrupções:** Implementação das funcionalidades dos botões com rotinas de interrupção (IRQ).  
- **Debouncing:** Tratamento de bouncing dos botões via software.  
- **Controle de LEDs:** Manipulação de LEDs comuns e endereçáveis.  
- **Organização:** Código bem estruturado e comentado.  

## 🚀 Como Executar
1. Dentro do VS Code, clone o repositório dentro de uma pasta do seu PC por meio do link:
   
   https://github.com/ateniltonjr/tarefa_27jan.git

   O clone do repositório pode ser realizado abrindo o terminal com o comando " Ctrl + ' " e digitar: git clone <link do repositório>.
   Ou manualmente atraves com do botão "clonar repositório" + copiar o link do repositório + enter.
   Certifique-se de que o caminho não é muito longo para evitar erros.
    
3. Certifique-se de que a placa BitDogLab está corretamente configurada.
4. Compile e carregue o código no microcontrolador RP2040 com botão Run.   
5. Execute o projeto e interaja com os botões para verificar as funcionalidades.  

## 📂 Estrutura do Repositório
  O repositório contém os arquivos e pastas:
      tarefa_27jan.'c'
      CMakeList. 'txt'
      diagram. 'json'
      pico_sdk_import. 'cmake'
      ws2818b.pio. 'h'
      diagram. 'json'
      wokwi. 'toml'
      README. 'MD'
      . 'gitignore'
      . 'VSCODE'

   Ao clonar dentro do VS Code será criado o arquivo build.

## 🎥 Demonstração  
Aqui está um video de demonstração do funcionamento do projeto na prática:
[Link para o vídeo de demonstração](#)  

## 🤝 Contribuições  
Sinta-se à vontade para contribuir com melhorias e sugestões!  

## 📄 Licença  
Este projeto está licenciado sob a [MIT License](LICENSE).  

## 📧 Contato  
Desenvolvido por Atenilton santos de Souza Júnior — [ateniltonjr09@gmail.com](ateniltonjr09@gmail.com)  
