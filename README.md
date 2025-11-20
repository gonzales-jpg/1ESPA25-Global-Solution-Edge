# VAVA – Vigilância Automática de Volume Ativo  
Sistema de Monitoramento Inteligente de Prateleiras

## Contexto Acadêmico  
O VAVA foi desenvolvido como parte da **Global Solution 2025 da FIAP**, dentro do tema **O Futuro do Trabalho**.  
A proposta do desafio incentiva o uso de tecnologia e inovação para criar soluções que tornem o trabalho **mais produtivo, inclusivo e sustentável**, alinhadas aos Objetivos de Desenvolvimento Sustentável:  
- Educação de Qualidade  
- Trabalho Decente  
- Inovação  
- Redução das Desigualdades  

---

## Contexto de Aplicação – Pequenos Comércios e Estoque Inteligente  
Em pequenos mercados, mercearias ou farmácias, a falta de controle visual do estoque nas prateleiras é comum.  
Quando um produto acaba e ninguém percebe:

- O cliente deixa de comprar  
- O comerciante perde venda  
- O trabalho dos repositores se torna reativo  
- A operação perde eficiência  

O **VAVA** foi criado exatamente para isso: um sistema que automatiza a leitura de estoque diretamente na prateleira.

---

## Sobre o Projeto  
O VAVA é um sistema embarcado que **estima automaticamente quantos produtos ainda restam em uma prateleira**, usando medição por distância.  

Ele é capaz de:

- Calcular de **0 a 8 unidades** restantes  
- Exibir no LCD mensagens como “8 produtos”, “5 produtos” ou “Prateleira vazia”  
- Sinalizar visualmente com LEDs quando a prateleira está cheia, quase acabando ou vazia  
- Registrar em **memória EEPROM** cada vez que o estoque zera, com data e hora  
- Permitir consulta ou limpeza dos registros via botão  

---

## Visão Geral do Circuito

(Insira aqui a imagem do seu protótipo — a imagem enviada no chat.)

---

## Componentes Utilizados  

| Quantidade | Componente | Função |
|-----------|------------|--------|
| 1 | Arduino UNO | Unidade de controle |
| 1 | HC-SR04 (Sensor Ultrassônico) | Medição de distância para estimar quantidade de produtos |
| 1 | LCD 16×2 (I²C) | Exibição do status da prateleira |
| 1 | RTC (DS1307/DS3231) | Registro de data e hora dos eventos |
| 1 | EEPROM interna | Armazenamento de histórico |
| 2 | LEDs (verde e vermelho) | Indicação rápida de estoque |
| 1 | Botão | Consulta e limpeza de registros |
| 1 | Protoboard + jumpers | Montagem |

---

## Funcionamento Geral  

### 1. Monitoramento Automático  
O VAVA mede continuamente a distância até os produtos.  
A partir da distância, o sistema converte o valor em uma **estimativa de unidades (0 a 8)**.

O LCD exibe algo como:

- “8 produtos”  
- “6 produtos”  
- “3 produtos”  
- “Prateleira vazia!”  

### 2. Indicação Visual  
- **Verde** → Prateleira cheia  
- **Vermelho** → Estoque zerado  

### 3. Registro em Memória  
Quando a prateleira chega a 0, o sistema:

- Acende o LED vermelho  
- Exibe “Prateleira vazia!”  
- Usa o RTC para registrar **data e hora exatas**  
- Armazena tudo na EEPROM, que mantém os dados mesmo desligando o Arduino  

### 4. Botão de Interação  
- **Clique rápido** → Consulta os registros salvos  
- **Pressão longa** → Limpa toda a memória para reiniciar o monitoramento  

---

## Entendendo o Sensor Ultrassônico  
O **HC-SR04** funciona emitindo um pulso ultrassônico pelo pino **TRIG**.  
Esse pulso bate nos produtos e retorna ao sensor pelo **ECHO**.

O Arduino calcula o tempo que o som levou para ir e voltar.  
Com esse tempo, converte o valor em **centímetros**, e então em **quantidade de produtos**.

Quando a distância ultrapassa o limite definido da prateleira, significa que **não existem mais itens**.

---

## Exemplo de Ciclo de Leitura  
O LCD atualiza continuamente:  

```
8 produtos
7 produtos
6 produtos
...
1 produto
Prateleira vazia!
```

Quando isso acontece:  
- LED vermelho acende  
- Registro é salvo na EEPROM  
- A data e hora ficam gravadas para análise futura  

---

## Aplicações e Benefícios  
O VAVA é um exemplo de automação de baixo custo que pode beneficiar:

- Mercados de bairro  
- Mercearias  
- Farmácias  
- Lojas de pequeno varejo  
- Estoques manuais  

Ele ajuda a:

- Reduzir perdas por falta de reposição  
- Aumentar a produtividade  
- Facilitar o trabalho dos repositores  
- Criar histórico de rupturas de estoque  
- Apoiar decisões de compra e reposição  

---

## VAVA e o Futuro do Trabalho  
O projeto se conecta diretamente às transformações do futuro do trabalho:

- Automatiza tarefas repetitivas  
- Libera pessoas para funções mais estratégicas  
- Exige novas habilidades técnicas  
- Cria oportunidades em dados, eletrônica e automação  

É uma demonstração prática de como soluções simples podem elevar a eficiência do trabalho cotidiano.

---

## Possíveis Evoluções Futuras  

- Integração com dashboards online  
- Notificações automáticas por aplicativo  
- Conexão com sistemas de estoque na nuvem  
- Previsão de demanda com IA  
- Rede de múltiplas prateleiras monitoradas simultaneamente  

---

## Integrantes do Projeto  

| Nome | Função |
|------|--------|
| Gabriel Ardito | Desenvolvimento |
| João Sarracine Faedrich | Desenvolvimento |
| João Pedro Gonzales | Desenvolvimento |