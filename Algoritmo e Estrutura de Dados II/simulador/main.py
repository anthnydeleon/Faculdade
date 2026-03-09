import json
from collections import defaultdict
from arvore_b import BTree

def carregar_dados():
    with open("produtos.json", "r", encoding="utf-8") as f:
        itens = json.load(f)
    with open("usuarios.json", "r", encoding="utf-8") as f:
        usuarios = json.load(f)
    with open("compras.json", "r", encoding="utf-8") as f:
        compras = json.load(f)
    return itens, usuarios, compras

def build_association_btree(compras, produto_alvo_id, t=3):
    contagem_produtos = defaultdict(int)
    contagem_pares = defaultdict(int)

    for compra in compras:
        produtos_unicos = set(compra["itens"])
        for p in produtos_unicos:
            contagem_produtos[p] += 1
        for p1 in produtos_unicos:
            for p2 in produtos_unicos:
                if p1 != p2:
                    contagem_pares[(p1, p2)] += 1

    def taxa_associacao(a, b):
        if contagem_produtos[a] == 0:
            return 0
        return contagem_pares[(a, b)] / contagem_produtos[a]

    arvore = BTree(t)
    for (a, b) in contagem_pares:
        if a == produto_alvo_id:
            taxa = taxa_associacao(a, b)
            arvore.insert(taxa, b)
    return arvore

def nome_do_item(itens, id_item):
    for item in itens:
        if item["id"] == id_item:
            return item["nome"]
    return "Item desconhecido"

if __name__ == "__main__":
    itens, usuarios, compras = carregar_dados()

    produto_alvo_id = 2
    arvore = build_association_btree(compras, produto_alvo_id)

    print(f"Produtos mais associados a '{nome_do_item(itens, produto_alvo_id)}':")
    top_assoc = arvore.recomendar_top_n(5)
    for taxa, id_produto in top_assoc:
        print(f"- {nome_do_item(itens, id_produto)} (taxa: {taxa:.2f})")
