# Joystick Web Server - Raspberry Pi Pico W

![Exemplo da Interface Web](https://lh4.googleusercontent.com/-CVs_02oIm3g/TYwFeJFzB-I/AAAAAAAAAcQ/shj5fbaKagA/s1600/ROSA+DOS+VENTOS.jpg)

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

## ⚡ Como Usar
1. **Conectar o Hardware**:
   - Joystick X → GPIO 26 (ADC0)
   - Joystick Y → GPIO 27 (ADC1)
   - Botão → GPIO 22 (pull-up interno)

2. **Compilar e Carregar**:
```bash
mkdir build && cd build
cmake ..
make
picotool load -f *.uf2
```

3. **Acessar a Interface**:
```bash
# No terminal serial:
Servidor acessível em: http://[IP-DO-PICO]
```

## 📂 Estrutura do Projeto
```
/
├── CMakeLists.txt
├── main.c               # Código principal
├── README.md
└── assets/
    ├── diagrama.png     # Diagrama de conexões
    └── screenshot.png   # Captura da interface
```

## 🚀 Melhorias Futuras
- [ ] Implementar atualização automática via AJAX
- [ ] Adicionar suporte a MQTT para nuvem
- [ ] Criar versão offline da rosa dos ventos

## 📌 Links
- [Vídeo Demo](https://drive.google.com/file/d/1BSFhj5zNNfcFWjSHbF-ScKewfieo6mbA/view)
- [Datasheet Pico W](https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf)

---

Desenvolvido por [Seu Nome] como parte do EmbarcaTech.
