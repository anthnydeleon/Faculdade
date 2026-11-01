# Guia de Estudo — Programação Funcional (Haskell)

> Resumo objetivo de todo o material do professor (Aulas 01 a 06) + lista 1, com pegadinhas e questões prováveis de prova.

---

## SUMÁRIO
1. Paradigma funcional e funções puras (Aula 01)
2. Cálculo λ (Aula 02)
3. Conceitos básicos de Haskell — tipos (Aula 03)
4. Conceitos básicos parte 2 — polimorfismo, guards, pattern matching, listas, recursão (Aula 04)
5. Funções de alta ordem — map, filter, fold, composição (Aula 05)
6. Tipos algébricos — type, data, newtype, record (Aula 06)
7. Pegadinhas mais comuns
8. Questões prováveis de prova (com respostas)
9. Lista 1 resolvida e comentada

---

# 1. PARADIGMA FUNCIONAL E FUNÇÕES PURAS

## Paradigmas (panorama)
- **Imperativo**: sequência de comandos, uso de `goto`.
- **Estruturado**: fluxo de controle, procedimentos e funções.
- **Orientado a objeto**: classes e objetos.
- **Declarativo**: especifica *o objetivo*, não *como fazer*.
- **Lógico**: fatos e regras (ex.: Prolog).
- **Funcional**: funções que **não alteram estados**, **dados imutáveis**.

> Funcional e Lógico são subtipos do paradigma **Declarativo**.

## O que caracteriza o paradigma funcional
Programar usando **apenas funções**, sem **efeitos colaterais** (não atribui variáveis, não modifica atributos, não altera estado externo).

**Vantagens** — um programa só com funções é mais fácil de:
- **Testar** (saída depende só da entrada)
- **Reusar**
- **Paralelizar** (sem estado compartilhado, sem condição de corrida)
- **Generalizar**
- Aumenta a **modularidade**

## Função pura
Função cujo único contato com o mundo externo é por **parâmetros (entrada)** e **retornos (saída)**. Não tem efeito colateral.

- Analogia com **função injetora** da matemática (ex.: `intToString`).
- **Transparência referencial**: uma expressão pode ser **substituída pelo seu resultado** em todos os lugares do código onde aparece, **sem alterar o comportamento** do programa (modelo de substituição). Analogia: resolver equações algébricas.

## Efeitos colaterais e modelo de substituição
- Efeitos colaterais tornam o raciocínio sobre o programa **mais difícil**.
- **Modelo de substituição**: efeitos ficam **locais**, eliminando a preocupação com sequências de atualizações de estado.
- **Objetivo do paradigma**: transformar funções com efeitos colaterais em **funções puras**.

> "Como fazer um loop sem alterar estado?" → com **recursão** (loops viram recursões).

---

# 2. CÁLCULO λ (LAMBDA)

## Definição
Sistema formal para expressar **computação**, baseado em **abstração de funções** e **aplicação**, usando apenas **atribuição de nomes** e **substituições**.
- Criado por **Alonzo Church** (década de 1930).
- Church foi orientador de doutorado de **Alan Turing**.
- É a **menor linguagem universal**: com apenas variáveis, abstração e aplicação consegue-se representar booleanos, números, condicionais, laços, recursão, etc.

## Sintaxe — 3 elementos
1. **Variável**: `x`, `y`, `z` — um nome que assume um valor durante a computação.
2. **Abstração** (função anônima / função λ): `λx.e` → em Haskell `\x -> e`
   - `x` é o **parâmetro formal**; `e` é o **corpo**. "Para qualquer `x`, compute `e`".
3. **Aplicação** (chamada de função): `e1 e2` → aplica o argumento `e2` na função `e1`, ou seja `e1(e2)`.

Exemplos:
- Função identidade: `λx.x` → `\x -> x`
- Função que devolve a identidade: `λx.(λy.y)` → `\x -> (\y -> y)`

> Conversão: `λ` → `\` e `.` → `->`

## Escopo de variáveis
- Em `\x -> e`, a expressão `e` é o **escopo** de `x`.
- Ocorrência de `x` dentro de `e` está **ligada (bound)**.
- `x` está **livre (free)** se não está dentro de uma abstração que a define.
- Se uma expressão **não tem variáveis livres**, ela é **fechada**.

## Semântica — reduções
Para avaliar/simplificar: aplicar reduções até não restar nenhuma possível.
- **Passo / redução α (alfa)**: **renomeia** um parâmetro formal (evita conflito de nomes).
- **Passo / redução β (beta)**: **aplica** a função (substitui o parâmetro pelo argumento).
  - Notação: `e1[x := e2]` = "`e1` com todas as ocorrências **livres** de `x` substituídas por `e2`".

## Redex e forma normal
- **Redex** (reducible expression): termo na forma `(\x -> e1) e2` (uma abstração aplicada a um argumento).
- **Forma normal**: termo que **não contém nenhum redex** (não dá pra reduzir mais).
- `e` avalia para `e'` se existe sequência `e => e1 => ... => e'` em que `e'` é forma normal.

## Avaliação que não termina — combinador Ω
`(\x -> x x) (\x -> x x)` → ao aplicar β, gera **ele mesmo** infinitamente. **Impossível** chegar a forma normal. É o **combinador Ω**.

## Programando com λ (codificações de Church)
Só temos funções, então tudo é representado com funções:

- **Booleanos**:
  - `TRUE = \x -> \y -> x` (devolve o primeiro)
  - `FALSE = \x -> \y -> y` (devolve o segundo)
  - `IF` é só uma aplicação de função; `NOT`, `AND`, `OR` se constroem a partir disso.
- **Tuplas / pares**: funções que guardam dois valores e aplicam um seletor.
- **Números de Church**: o número `N` é codificado como **aplicar uma função `f` N vezes** a um `x`.
  - `0 = \f -> \x -> x` (aplica `f` zero vezes)
  - `1 = \f -> \x -> f x`
  - `2 = \f -> \x -> f (f x)`
  - Função **incremento (INC)** adiciona uma aplicação de `f`; **soma** aplica INC repetidamente.

## Recursão no cálculo λ — ponto fixo e combinador Y
No cálculo λ as funções **não têm nome**, então não dá pra "chamar a si mesma" diretamente.
- Solução: **ponto fixo**. `p` é ponto fixo de `fat` se `p ≡ fat p` (e `p n ≡ fat p n`).
  - Ex.: `3! ≡ p 3 ≡ fat p 3 ≡ 6`.
  - (Ponto fixo = elemento do domínio mapeado para ele mesmo pela função.)
- **Combinador Y**: expressão λ que **devolve um ponto fixo** de uma função λ. É uma adaptação do combinador Ω que, além das reduções infinitas, incorpora a aplicação de `f` (`A = f A`).
- Com o combinador Y é possível definir, por exemplo, o **fatorial** recursivamente.

---

# 3. CONCEITOS BÁSICOS DE HASKELL (Aula 03)

## Características da linguagem
- **Puramente funcional**
- Código **conciso e declarativo**
- **Imutabilidade**
- **Funções recursivas**
- **Funções de alta ordem** (recebem/retornam funções)
- **Tipos polimórficos** (funcionam para classes de tipos)
- **Avaliação preguiçosa (lazy)**: resultados computados **apenas quando usados**

## Funções
- Aplicação de função tem a **maior precedência**. `f x + 1` é `(f x) + 1`.
- Notação de tipo: `v :: T` ("`v` é do tipo `T`").

## Convenções
- **Funções/argumentos**: começam com **letra minúscula**, depois letras, dígitos, `_` e `'` (aspas simples). Ex.: `minhaFunc'`.
- **Nomes reservados**: `case, class, data, default, deriving, do, else, foreign, if, import, in, infix, infixl, infixr, instance, let, module, newtype, of, then, type, where`.
- **Listas**: nomeadas acrescentando `s` ao que representam — número `n` → lista `ns`; variável `x` → `xs`; lista de listas de char → `css`.
- **Layout**: indentação importa (como Python). **Use espaços, não Tab.**
- **Comentários**: `--` em uma linha; `{- ... -}` em várias.

## Tipos básicos
| Tipo | Descrição |
|---|---|
| `Bool` | `True`/`False`; operadores `&&`, `||`, `not` |
| `Char` | caractere Unicode, aspas simples: `'a'` |
| `String` | sequência de chars, aspas duplas: `"Olá Mundo"` (é `[Char]`) |
| `Int` | inteiro **precisão fixa 64 bits** (−2⁶³ a 2⁶³−1) |
| `Integer` | inteiro **precisão arbitrária** (limite = memória), mais lento |
| `Float` | ponto flutuante simples (~7 dígitos) |
| `Double` | ponto flutuante duplo (~16 dígitos) |

## Listas
- Sequência de elementos **do mesmo tipo**, entre `[ ]` separados por vírgula.
- Tipo de lista de `T` é `[T]`. Ex.: `[1,2,3] :: [Int]`, `['o','l','a'] :: [Char]`.
- Listas de listas: `[[1,2,3],[4,5]] :: [[Int]]`.
- O tipo **não** especifica tamanho; sem limite de tamanho.

## Tuplas
- Sequência finita com **zero ou mais tipos diferentes**. Ex.: `(1.0,"Sim",False) :: (Double,String,Bool)`.
- O tipo da tupla **especifica seu tamanho**; deve ter tamanho finito.

> **Lista vs Tupla** (pegadinha clássica):
> - Lista: tamanho variável, **todos do mesmo tipo**, tipo não diz tamanho.
> - Tupla: tamanho fixo, **tipos podem ser diferentes**, tipo diz o tamanho.

## Funções (tipos)
- `T1 -> T2`: mapeia tipo `T1` em `T2`. Ex.: `not :: Bool -> Bool`, `even :: Int -> Bool`.
- Múltiplos argumentos: separados por `->`; o **último** é o tipo de retorno.
  - `soma :: Int -> Int -> Int`.

---

# 4. CONCEITOS BÁSICOS — PARTE 2 (Aula 04)

## Polimorfismo
- **Variável de tipo** (minúscula, ex.: `a`): permite uma função funcionar para vários tipos.
  - Ex.: `length :: [a] -> Int`.
- **`where`**: define valores/funções auxiliares locais.

## Condicionais
```haskell
if cond then e1 else e2
```
- `else` é **obrigatório** (expressão tem que ter valor sempre).
- Restrições de classe: ex.: `Ord a` (ordenável), `Num a` (numérico).
- Podem ser **encadeados** (`if ... then ... else if ... then ... else ...`).

## Expressões guardadas (Guards)
Lido como "tal que". Avaliadas **de cima para baixo**, retorna o **primeiro** `True`.
```haskell
raiz x
  | x < 0     = error "negativo"
  | otherwise = sqrt x
```
- `otherwise` é apenas `True` (caso padrão).
- `error "..."` **interrompe a execução** do programa.

## Pattern matching (casamento de padrões)
- Não precisa enumerar todos os casos: pode definir **casos especiais**.
- Padrões avaliados **do primeiro ao último** (ordem importa!).
```haskell
nulo []    = True
nulo (_:_) = False
```

## Expressões λ (funções anônimas)
```haskell
somaMultX :: Num a => a -> (a -> a)
somaMultX x = \y -> x + x * y
somaMult2 = somaMultX 2   -- aplicação parcial
```

## Operadores
- Forma **infixa** (`x + y`) ou de **função** (`(+) x y`).
- Operador entre crases vira infixo: `mod 10 3` = ``10 `mod` 3``.

## Listas — operador `(:)` e funções
- `(:)` (**cons**): concatena **um elemento** com **uma lista**. `1 : [2,3]` = `[1,2,3]`.
  - `[1,2,3]` é açúcar para `1:(2:(3:[]))`.
- **Lista infinita** (avaliação preguiçosa): `[1..]` é só uma "promessa"; gera elementos conforme necessário, **não ocupa memória infinita**.

| Função | O que faz | Exemplo |
|---|---|---|
| `!!` | i-ésimo elemento (índice começa em **0**) | `[10,20,30] !! 1` = `20` |
| `head` | primeiro elemento | `head [1,2,3]` = `1` |
| `tail` | lista sem o primeiro (a cauda) | `tail [1,2,3]` = `[2,3]` |
| `take n` | primeiros `n` | `take 2 [1,2,3]` = `[1,2]` |
| `drop n` | remove os primeiros `n` | `drop 2 [1,2,3]` = `[3]` |
| `length` | tamanho | `length [1,2,3]` = `3` |
| `sum` | somatória | `sum [1,2,3]` = `6` |
| `product` | produtória | `product [1,2,3]` = `6` |
| `++` | concatena duas **listas** | `[1,2]++[3]` = `[1,2,3]` |
| `zip` | junta duas listas em pares; **para na menor** | `zip [1,2,3] ['a','b']` = `[(1,'a'),(2,'b')]` |
| `and`/`or` | True se todos/algum forem True | `and [True,False]` = `False` |

> **Pegadinha `!!` vs `:`** — `:` adiciona elemento à frente; `!!` acessa por índice. Não confundir.
> **Pegadinha `++` vs `:`** — `++` é lista++lista; `:` é elemento:lista. `1 ++ [2]` dá **erro de tipo**.

## Padrões de lista (pattern matching em listas)
- Lista vazia: `[]`
- Um elemento: `(x:[])` ou `[x]`
- Cabeça + cauda: `(x:xs)`

```haskell
tamanho []     = 0
tamanho (_:xs) = 1 + tamanho xs
```

## Compreensão de listas (list comprehension)
Matemática: *x² para todo x de 1 a 5*.
```haskell
[x^2 | x <- [1..5]]   -- [1,4,9,16,25]
```
- `x <- [1..5]` é a **expressão geradora**.
- **Múltiplos geradores** = laços **encadeados** (o último varia mais rápido):
```haskell
[(x,y) | x <- [1..4], y <- [4..5]]
-- [(1,4),(1,5),(2,4),(2,5),...]  -- y varia mais rápido
```
- Um gerador pode **depender** do anterior:
```haskell
[(i,j) | i <- [1..5], j <- [i+1..5]]
```
- **Guards como filtro**:
```haskell
[x | x <- [1..10], even x]   -- [2,4,6,8,10]
```

Exemplos a memorizar:
```haskell
[toLower c | c <- "OLA MUNDO"]        -- "ola mundo"
[(x, even x) | x <- [1,2,3]]          -- [(1,False),(2,True),(3,False)]
concat xss   = [x | xs <- xss, x <- xs]
length xs    = sum [1 | _ <- xs]
divisores n  = [x | x <- [1..n], n `mod` x == 0]
primo n      = divisores n == [1,n]
primos n     = [x | x <- [1..n], primo x]
todosOsPrimos = [x | x <- [1..], primo x]   -- lista infinita!
pairs xs     = zip xs (tail xs)              -- pares adjacentes
sorted xs    = and [x <= y | (x,y) <- pairs xs]
```

## Recursão
- Tem **1 ou mais casos base** (para terminar) — **sempre primeiro**.
- Haskell avalia por **substituição** (não guarda estado em pilha automaticamente como linguagens imperativas).
```haskell
fatorial :: Integer -> Integer
fatorial 0 = 1
fatorial 1 = 1
fatorial n = n * fatorial (n-1)
```
Avaliação: `fatorial 4 => 4*(3*(2*1)) => 24`.

## Recursão na cauda (tail recursion)
A chamada recursiva é a **última operação** (nada acontece depois dela). Usa **pilha de tamanho constante**.
```haskell
-- SEM recursão na cauda (precisa somar n DEPOIS da chamada):
soma (n:ns) = n + soma ns

-- COM recursão na cauda (acumulador):
soma ns = soma' ns 0
  where soma' []     s = s
        soma' (n:ns) s = soma' ns (n+s)
```
Exemplo `gcd` (já é tail recursive):
```haskell
gcd 0 b = b
gcd a 0 = a
gcd a b = gcd b (a `rem` b)
-- gcd 48 18 => gcd 18 12 => gcd 12 6 => gcd 6 0 => 6
```

## Recursão múltipla
Função se chama **mais de uma vez** no retorno. Ex.: Fibonacci.
```haskell
fib 0 = 1
fib 1 = 1
fib n = fib (n-1) + fib (n-2)
```

## Receita para escrever funções recursivas
1. Defina a **assinatura**.
2. **Enumere** os casos.
3. Defina os **casos simples** (base).
4. Defina os **casos restantes**.
5. **Generalize e simplifique** (use `_` para o que não importa).

---

# 5. FUNÇÕES DE ALTA ORDEM (Aula 05)

## Definição
Função de alta ordem = **recebe função(ões) como argumento** e/ou **devolve uma função**.

## Funções como dado / aplicação parcial
```haskell
soma :: Int -> (Int -> Int)   -- mesma coisa que Int -> Int -> Int
soma x y = x + y
soma3 = soma 3       -- aplicação parcial
soma3 2              -- 5
```
> **Currying**: `Int -> Int -> Int` é, na verdade, `Int -> (Int -> Int)` — toda função de N argumentos é uma cadeia de funções de 1 argumento. Por isso a **aplicação parcial** sempre funciona.

```haskell
duasVezes :: (a -> a) -> a -> a
duasVezes f x = f (f x)
duasVezes (*2) 3        -- 12
duasVezes reverse [1,2,3] -- [1,2,3]
```
- `(*2)` e `(>5)` são **seções** (funções parciais de operador).

## map
Aplica uma função a **cada** elemento. Substitui o padrão `[f x | x <- xs]`.
```haskell
map :: (a -> b) -> [a] -> [b]
map f []     = []
map f (x:xs) = f x : map f xs

map (+1) [1,2,3]              -- [2,3,4]
map even [1,2,3]             -- [False,True,False]
map reverse ["ola","mundo"]  -- ["alo","odnum"]
```

## filter
Mantém só os elementos que satisfazem o predicado.
```haskell
filter :: (a -> Bool) -> [a] -> [a]
filter p []     = []
filter p (x:xs) | p x       = x : filter p xs
                | otherwise = filter p xs

filter even [1..10]                -- [2,4,6,8,10]
filter (>5) [1..10]                -- [6,7,8,9,10]
filter (/= ' ') "abc def"          -- "abcdef"
filter (\s -> length s < 4) ["ola","mundo"]  -- ["ola"]
```

## map + filter juntos e operador `$`
```haskell
somaQuadPares ns = sum (map (^2) (filter even ns))
-- com operador $ (remove parênteses, executa de baixo p/ cima):
somaQuadPares ns = sum
                 $ map (^2)
                 $ filter even ns
```
- **`$`**: separa aplicações de função, permitindo remover parênteses. `f $ x = f x`, mas com **menor precedência**.

## Outras de alta ordem
```haskell
all even [2,4,6,8]          -- True   (todos satisfazem)
any odd  [2,4,6,8]          -- False  (algum satisfaz)
takeWhile even [2,4,6,7,8]  -- [2,4,6]  (pega enquanto verdadeiro)
dropWhile even [2,4,6,7,8]  -- [7,8]    (descarta enquanto verdadeiro)
```

## Folding (foldr e foldl)
Generalizam a recursão "combinar todos os elementos num resultado".

- **`foldr`** ("dobra pela direita"): `foldr (⊕) z [a,b,c] = a ⊕ (b ⊕ (c ⊕ z))`
```haskell
foldr :: (a -> b -> b) -> b -> [a] -> b
sum'     = foldr (+) 0
product  = foldr (*) 1
length   = foldr (\_ n -> n + 1) 0
```
- **`foldl`** ("dobra pela esquerda" — associa à esquerda; relacionado a recursão na cauda):
  `foldl (⊕) z [a,b,c] = ((z ⊕ a) ⊕ b) ⊕ c`

### foldr vs foldl — como escolher (cai em prova!)
- Lista pode ser **infinita** → **`foldr`** (consegue curto-circuitar e produzir resultado).
- Operador pode gerar **curto-circuito** (ex.: `&&`, `||`) → **`foldr`**.
  - `foldr`: `False && (False && (...))` → para no primeiro `False`.
  - `foldl`: `((False && False) && False)` → precisa percorrer tudo.
- Caso contrário → **`foldl`** (pode gerar otimizações).

## Composição de funções `(.)`
- Matemática: `f.g` define `z(x) = f(g(x))`.
- Haskell: `(f . g) x = f (g x)`.
- Propriedades:
  - **Associativa**: `(f . g) . h == f . (g . h)`
  - **Elemento neutro `id`**: `f . id == id . f == f`

---

# 6. TIPOS ALGÉBRICOS (Aula 06)

## `type` — apelido (sinônimo)
```haskell
type Preco = Double
type Nome  = String
type Coord = (Double, Double)
```
- Substituição **em tempo de compilação**. `String` é, na verdade, `type String = [Char]`.
- Aceita **tipos paramétricos**: `type Par a = (a, a)`.

## `data` — tipo novo "de verdade"
Quatro "formas" de construir tipos: **Soma**, **Produto**, **Recursivo**, **Exponencial** (funções).

### Tipo Soma ("ou") — alternativas
```haskell
data Bool = True | False
data Cor  = Vermelho | Verde | Azul
```

### Tipo Produto ("e") — agrupa campos
```haskell
data Ponto = MkPonto Double Double  deriving (Show)
--           ^construtor  ^tipos encapsulados
```
- **Construtor** = função implícita usada para **criar um valor** do tipo (`MkPonto 1.0 2.0`).
- `deriving (Show)` → torna o tipo **imprimível**.

### Misturando soma e produto
```haskell
data Forma = Circulo Double | Retangulo Double Double
-- Circulo e Retangulo são funções construtoras
area :: Forma -> Double
area (Circulo r)      = pi * r * r
area (Retangulo b h)  = b * h
```

## Tipos paramétricos (com parâmetro de tipo)
```haskell
data Maybe a  = Nothing | Just a          -- representa "pode falhar"
data Either a b = Left a | Right b        -- retorna 2 tipos diferentes
```
- `Maybe`: `Nothing` (sem valor) ou `Just x` (com valor).
- `Either`: `Left` (geralmente erro) ou `Right` (geralmente sucesso).

## `type` vs `data` (tabela — cai em prova!)
| | `type` | `data` |
|---|---|---|
| O que é | apelido (substituição em compilação) | **tipo novo** |
| Eficiência | mais eficiente | menos eficiente |
| Segurança | **menos seguro** | **mais seguro** |

> Misturar apelido `type` com o tipo base pode passar batido pelo compilador (menos seguro). `data` cria tipo distinto → trocar dá **erro de compilação** (mais seguro).

## `newtype` — meio-termo
- Como `data`: **cria um novo tipo** (checagem de tipo em compilação).
- Como `type`: depois de compilado é **substituído** (sem custo em tempo de execução).
- **Limitação**: **não pode ser usado com tipo Soma** (só um construtor, com **um** campo).

## Tipo recursivo
Usa o **próprio tipo** na definição.
```haskell
-- Lista própria
data Lista a = Vazia | Cons a (Lista a)

-- Árvore binária
data Arvore a = Folha | No (Arvore a) a (Arvore a)

contem :: Eq a => Arvore a -> a -> Bool
contem Folha _ = False
contem (No esq v dir) x = x == v || contem esq x || contem dir x
--                                ^operador "ou"
```

## Record type (sintaxe de registro)
Outra forma de definir tipo **produto**, dando **nome aos campos**. Cria automaticamente as funções de acesso.
```haskell
data Coord = MkCoord { coordX :: Double
                     , coordY :: Double
                     , coordZ :: Double }
-- cria coordX, coordY, coordZ que retornam o valor do campo
```
- Acesso: `coordX meuPonto`.
- Vantagem: não precisa lembrar a **ordem** dos parâmetros.

---

# 7. PEGADINHAS MAIS COMUNS

1. **`Int` vs `Integer`**: `Int` é fixo (64 bits, pode estourar/overflow); `Integer` é arbitrário (não estoura, mas é mais lento).
2. **`:` vs `++`**: `:` é `elemento : lista`; `++` é `lista ++ lista`. `1 ++ [2,3]` → **erro de tipo**.
3. **Índice de `!!` começa em 0.** `[10,20] !! 2` → **erro** (fora do limite).
4. **Ordem dos padrões/guards importa**: o primeiro que casa é usado. Caso base **sempre antes** do recursivo.
5. **`zip` para na menor lista**: `zip [1,2,3] "ab"` = `[(1,'a'),(2,'b')]`.
6. **Lista = mesmo tipo; Tupla = pode misturar tipos.** Tipo da tupla fixa o tamanho; tipo da lista não.
7. **Ordem dos geradores na compreensão**: o **último** gerador varia mais rápido (laço mais interno).
   - `[(x,y) | x<-[1..2], y<-[3..4]]` ≠ `[(x,y) | y<-[3..4], x<-[1..2]]` (ordem dos pares muda).
8. **`if` exige `else`** — sempre tem que retornar um valor.
9. **`foldr` para listas infinitas / curto-circuito**; `foldl` quando precisa percorrer tudo.
10. **Lista infinita só funciona por avaliação preguiçosa** — `take 5 [1..]` funciona; `length [1..]` **trava**.
11. **Aplicação de função tem maior precedência**: `f x + 1` = `(f x) + 1`, não `f (x+1)`.
12. **`type` não cria tipo novo** (menos seguro); **`data` cria** (mais seguro); **`newtype`** cria tipo novo de 1 construtor/1 campo.
13. **Combinador Ω `(\x->x x)(\x->x x)`** nunca atinge forma normal (loop infinito).
14. **Redução β substitui só ocorrências LIVRES** da variável.
15. **Currying**: `f a b` é `(f a) b`. Toda função de vários args pode ser parcialmente aplicada.

---

# 8. QUESTÕES PROVÁVEIS DE PROVA (com respostas)

### Conceituais

**Q1. O que caracteriza o paradigma funcional e cite vantagens.**
R: Programar usando apenas funções, sem efeitos colaterais e com dados imutáveis. Vantagens: mais fácil de testar, reusar, paralelizar, generalizar e maior modularidade.

**Q2. O que é uma função pura?**
R: Função cujo único contato com o exterior é por parâmetros e retorno; não tem efeito colateral e respeita a transparência referencial (pode ser substituída pelo resultado sem mudar o comportamento).

**Q3. O que é transparência referencial?**
R: Propriedade em que uma expressão pode ser trocada pelo seu resultado em qualquer lugar do código sem alterar o comportamento do programa.

**Q4. O que é recursão na cauda?**
R: Quando a chamada recursiva é a última operação da função (não há cálculo após ela). Usa pilha de tamanho constante. Geralmente implementada com um acumulador.

**Q5. O que é cálculo λ?**
R: Sistema formal de Alonzo Church (1930s) para expressar computação via abstração e aplicação de funções, usando apenas nomes e substituições. Possui 3 elementos: variáveis, abstração e aplicação.

**Q6. Diferença entre redução α e β.**
R: α renomeia o parâmetro formal; β aplica a função, substituindo as ocorrências livres do parâmetro pelo argumento.

**Q7. O que é forma normal / redex?**
R: Redex é um termo `(\x -> e1) e2` (pode ser reduzido). Forma normal é um termo sem nenhum redex (não dá pra reduzir mais).

**Q8. Diferença entre `type`, `data` e `newtype`.**
R: `type` é apelido (substituição em compilação, eficiente, menos seguro); `data` cria tipo novo (menos eficiente, mais seguro, permite soma/produto); `newtype` é meio-termo (tipo novo com 1 construtor e 1 campo, sem custo em execução, não funciona com tipo Soma).

**Q9. Diferença entre `foldr` e `foldl` e quando usar cada.**
R: `foldr` associa à direita `a⊕(b⊕(c⊕z))`; `foldl` à esquerda `((z⊕a)⊕b)⊕c`. Use `foldr` para listas infinitas ou operadores com curto-circuito; `foldl` caso contrário (otimizações).

**Q10. Lista vs tupla.**
R: Lista tem tamanho variável e todos os elementos do mesmo tipo; o tipo não informa o tamanho. Tupla tem tamanho fixo, tipos podem ser diferentes; o tipo informa o tamanho.

### Avaliação de expressões λ

**Q11. Reduza `(\x -> x) y`.** → **R: `y`**

**Q12. Reduza `(\x -> x x) (\y -> y)`.**
R: substitui `x` por `(\y->y)`: `(\y->y)(\y->y)` → `(\y->y)` (identidade aplicada à identidade = identidade).

**Q13. Qual termo NÃO está na forma normal?**
`a) x  b) x y  c) (\x -> x) y  d) x (\y -> y)`
R: **(c)** — é um redex (abstração aplicada a argumento).

**Q14. Termo sintaticamente incorreto?**
`a) \(\x -> x) -> y   b) \x -> x x   c) \x -> x (y x)`
R: **(a)** — o parâmetro de uma λ tem que ser uma variável, não uma expressão `(\x->x)`.

### "O que retorna / qual a saída?"

```haskell
[x^2 | x <- [1..5]]             -- [1,4,9,16,25]
[toLower c | c <- "OLA MUNDO"]  -- "ola mundo"
[(x, even x) | x <- [1,2,3]]    -- [(1,False),(2,True),(3,False)]
zip [1,2,3] ['a','b','c','d']   -- [(1,'a'),(2,'b'),(3,'c')]
map (+1) [1,2,3]                -- [2,3,4]
filter even [1..10]             -- [2,4,6,8,10]
takeWhile even [2,4,6,7,8]      -- [2,4,6]
dropWhile even [2,4,6,7,8]      -- [7,8]
all even [2,4,6,8]              -- True
any odd  [2,4,6,8]              -- False
foldr (+) 0 [1,2,3]             -- 6
take 3 [1..]                    -- [1,2,3]
```

### Implementação (treine escrever do zero)

**Q15. Implemente `length` com recursão.**
```haskell
tamanho :: [a] -> Int
tamanho []     = 0
tamanho (_:xs) = 1 + tamanho xs
```

**Q16. Implemente `map` e `filter` recursivamente.** (ver Aula 05 acima)

**Q17. Implemente `fatorial`.**
```haskell
fatorial 0 = 1
fatorial n = n * fatorial (n-1)
```

**Q18. Defina `length` usando `foldr`.**
```haskell
length = foldr (\_ n -> n + 1) 0
```

**Q19. Defina `sorted` (lista ordenada?).**
```haskell
sorted :: Ord a => [a] -> Bool
sorted xs = and [x <= y | (x,y) <- zip xs (tail xs)]
```

**Q20. Defina `primo` usando `divisores`.**
```haskell
divisores n = [x | x <- [1..n], n `mod` x == 0]
primo n     = divisores n == [1,n]
```

---

# 9. LISTA 1 — RESOLVIDA E COMENTADA

**1. O que caracteriza o paradigma funcional e vantagens?** → ver Q1.

**2. O que é função pura?** → ver Q2.

**3. O que é recursão na cauda?** → ver Q4.

**4. O que é cálculo λ?** → ver Q5.

**5. Resultado de `(\x -> x) y`?** → **`y`** (identidade aplicada a `y`).

**6. Merge de duas listas ordenadas:**
```haskell
merge :: Ord a => [a] -> [a] -> [a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) (y:ys)
  | x <= y    = x : merge xs (y:ys)
  | otherwise = y : merge (x:xs) ys
```
*Ideia*: compara as cabeças, pega a menor, recursão no resto. Casos base: lista vazia retorna a outra.

**7. Produto interno (cartesiano) de duas listas:**
```haskell
prodInt :: Num a => [a] -> [a] -> a
prodInt xs ys = sum [x*y | (x,y) <- zip xs ys]
```
*Ideia*: `zip` pareia, multiplica cada par, `sum` soma tudo. (É o produto escalar/interno.)

**8. O que a função `que` faz?**
```haskell
que :: String -> String -> Int
que p t = length
        $ filter (==p)
        $ words
        $ filter (/='.')
        $ filter (/='!')
        $ filter (/='?') t
```
R: **Conta quantas vezes a palavra `p` aparece no parágrafo `t`.** Lê de baixo p/ cima: remove `?`, `!`, `.`, separa em palavras (`words`), filtra as iguais a `p`, conta (`length`).

**9. Record type para produto de mercado + melhor custo/benefício:**
```haskell
data Produto = MkProduto
  { cod   :: Int
  , nome  :: String
  , peso  :: Double
  , preco :: Double }

maisBarato :: Produto -> Produto -> Int
maisBarato p1 p2
  | (preco p1 / peso p1) <= (preco p2 / peso p2) = cod p1
  | otherwise                                    = cod p2
```
*Ideia*: melhor custo/benefício = menor `preço/peso`. O record cria automaticamente `cod`, `nome`, `peso`, `preco`.

---

## CHECKLIST FINAL ANTES DA PROVA
- [ ] Sei diferenciar função pura/impura e explicar transparência referencial.
- [ ] Sei reduzir expressões λ (α e β) e identificar redex/forma normal.
- [ ] Conheço as codificações de Church (booleanos, números) e o combinador Y/Ω.
- [ ] Decoro os tipos básicos e a diferença Int/Integer, Float/Double.
- [ ] Sei a diferença lista × tupla.
- [ ] Escrevo guards, pattern matching e recursão (inclusive na cauda).
- [ ] Domino compreensão de listas (ordem dos geradores, filtros).
- [ ] Sei usar/implementar map, filter, foldr, foldl, composição e `$`.
- [ ] Conheço type × data × newtype e record type.
- [ ] Sei avaliar "qual a saída" das funções de lista mais comuns.
