# Joystick Web Server - Raspberry Pi Pico W

![Exemplo da Interface Web]([https://lh4.googleusercontent.com/-CVs_02oIm3g/TYwFeJFzB-I/AAAAAAAAAcQ/shj5fbaKagA/s1600/ROSA+DOS+VENTOS.jpg](https://drive.google.com/file/d/1CTPZbaDW87giO-iw4OvSNdMwouEncB_b/view?usp=sharing))

## 📝 Descrição
Projeto desenvolvido para monitorar um joystick analógico via servidor web embarcado no Raspberry Pi Pico W. Exibe:
- Posição dos eixos X/Y (-1.0 a +1.0)
- Estado do botão (pressionado/solto)
- Direção cardinal (Norte, Sul, etc.) com ângulo (0°-360°)
- Compasso visual interativo

> **Nota:** A atualização requer recarregar a página manualmente (F5).

## 🛠️ Tecnologias
### Hardware
- Raspberry Pi Pico W
- Joystick analógico (eixos X/Y + botão)

### Software
- Linguagem C (SDK oficial da Raspberry Pi)
- Servidor HTTP com endpoints:
  - `/` → Página HTML com compasso
  - `/data` → API JSON (`{"x":0.5, "y":-0.3, "button":"Solto", "direction":"Nordeste", "angle":45.0}`)
- CSS embutido para interface

## 🎯 Funcionalidades
✅ **Leitura Analógica**
- Normalização dos valores ADC (-1.0 a +1.0)
- Debounce do botão

✅ **Processamento de Dados**
- Cálculo do ângulo com `atan2f`
- Mapeamento para 8 direções cardeais

✅ **Interface Web**
- Rosa dos ventos com marcador dinâmico
- Exibição numérica dos valores
- Design responsivo

## 🖥️ Interface Web
**Componentes:**
1. **Compasso Visual**
   - Imagem de fundo com rosa dos ventos
   - Marcador vermelho que se move conforme o joystick

2. **Painel de Dados**
   - Eixo X/Y
   - Estado do botão
   - Direção e ângulo

3. **Atualização Manual**
   - Recarregue a página (F5) para ver mudanças

## 📌 Links
- [Vídeo Demostração](https://drive.google.com/file/d/1BSFhj5zNNfcFWjSHbF-ScKewfieo6mbA/view)


Desenvolvido por Cleber Henrique Lacerda Duarte como parte do EmbarcaTech.
