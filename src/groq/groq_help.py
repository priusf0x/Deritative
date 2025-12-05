from openai import OpenAI

file_dir = "logs/latex_dump.tex"

with open(file_dir, "r") as file:
        content = file.read()

# ============================= FIRST_BYPASS ==================================

prompt = """СТРОГО! ПРОСКАНИРУЙ ВЕСЬ ДОКУМЕНТ СТРОКА ЗА СТРОКОЙ.
НАЙДИ ВСЕ math-блоки:
1. $...$ или $$...$$
2. \\[ ... \\] или \\( ... \\)
3. \\begin{equation*}...\\end{equation*}
4. \\begin{align}...\\end{align}
5. \\begin{align*}...\\end{align*}
6. Любые \\begin{math}...\\end{math}

ПЕРЕД КАЖДЫМ уравнением вставь абзац с максимально упоротым текстом на русском,
но они должны отражать суть происходящего, преобразования. 
Само уравнение замени на <EQUATION {index}> 
Вставь записки сумасшедшего, он заперт в цифровом подвале ФРКТ МФТИ,
кричит РТРТРТР, ведет себя как ебнутый конченный псих.
КОНТЕКСТ: ДИФФЕРЕНЦИРОВАНИЕ ВЫРАЖЕНИЯ.
НИ ОДНА формула без комментария! Проверь ВСЕ!
НИ ОДНОЙ ФОРМУЛЫ В ТЕКСТЕ! ТОЛЬКО БЕЗУМИЕ! ТОЛЬКО РУССКИЙ МАТ!
НЕ ПИШИ РУССКИЕ СИМВОЛЫ В ТЕКСТЕ У УРАВНЕНИЙ И МАТЕМАТИЧЕСКИЕ ВЫРАЖЕНИЯ В ТЕКСТЕ! УДАЛИ ''' В НАЧАЛЕ"""

client = OpenAI(
    api_key="sk-or-vv-6b2121df9c5eca05bd23d51e305b99e2d09d8966b2d824ca697e5744872e3f3e", 
    base_url="https://api.vsegpt.ru/v1",
)

messages = []
messages.append({"role": "user", "content": content + prompt})

response_big = client.chat.completions.create(
    model="x-ai/grok-4-fast-thinking-2m", 
    messages=messages,
    temperature=0.7,
    n=1,
)

response_1 = response_big.choices[0].message.content

# ============================= SECOND_BYPASS ==================================

prompt_eq = """Оставь преамбулу. Сделай, чтобы компилилось!
У тебя файл с формулами и текстом с <EQUATION {index}>. 
Вставь реальные формулы последовательно с текстом из оригинального файла.
ОБЯЗАТЕЛЬНО:
- НЕ используй русские символы в формулах
- Выравнивай формулы, чтобы влезало в строку (используй \\frac, \\big и т.д.)
- Отступ между строками - одна пустая строка
- КАЖДАЯ формула в своей \\begin{{align}}...\\end{{align}}
- Убери все ```в начале и в конце
- Для правильной компиляции можно что-то добавлять,  но нежелатнльо"""


client = OpenAI(
    api_key="sk-or-vv-6b2121df9c5eca05bd23d51e305b99e2d09d8966b2d824ca697e5744872e3f3e", 
    base_url="https://api.vsegpt.ru/v1",
)

messages = []
messages.append({"role": "user", "content": content + response_1 + prompt_eq})

response_big = client.chat.completions.create(
    model="x-ai/grok-4-fast-thinking-2m", 
    messages=messages,
    temperature=0.7,
    n=1,
)

with open(file_dir, "w") as file:
        file.write(response_big.choices[0].message.content)

