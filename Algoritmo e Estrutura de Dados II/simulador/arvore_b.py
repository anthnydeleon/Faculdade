class TESTNode:
    def __init__(self, t, leaf=False):
        self.t = t
        self.leaf = leaf
        self.keys = []  # lista de tuplas (taxa, produto_associado)
        self.children = []

    def insert_non_full(self, taxa, produto):
        i = len(self.keys) - 1
        if self.leaf:
            self.keys.append((None, None))
            while i >= 0 and taxa < self.keys[i][0]:
                self.keys[i + 1] = self.keys[i]
                i -= 1
            self.keys[i + 1] = (taxa, produto)
        else:
            while i >= 0 and taxa < self.keys[i][0]:
                i -= 1
            i += 1
            if len(self.children[i].keys) == 2 * self.t - 1:
                self.split_child(i)
                if taxa > self.keys[i][0]:
                    i += 1
            self.children[i].insert_non_full(taxa, produto)

    def split_child(self, i):
        t = self.t
        y = self.children[i]
        z = TESTNode(t, y.leaf)

        z.keys = y.keys[t:]
        y.keys = y.keys[:t - 1]

        if not y.leaf:
            z.children = y.children[t:]
            y.children = y.children[:t]

        self.children.insert(i + 1, z)
        self.keys.insert(i, y.keys.pop())

    def reverse_traverse(self, resultados, n):
        for i in range(len(self.keys) - 1, -1, -1):
            if not self.leaf:
                self.children[i + 1].reverse_traverse(resultados, n)
            if len(resultados) < n:
                resultados.append(self.keys[i])
        if not self.leaf and len(resultados) < n:
            self.children[0].reverse_traverse(resultados, n)


class TEST:
    def __init__(self, t):
        self.root = TESTNode(t, True)
        self.t = t

    def insert(self, taxa, produto):
        r = self.root
        if len(r.keys) == 2 * self.t - 1:
            s = TESTNode(self.t, False)
            s.children.insert(0, r)
            s.split_child(0)
            i = 0
            if s.keys[0][0] < taxa:
                i += 1
            s.children[i].insert_non_full(taxa, produto)
            self.root = s
        else:
            r.insert_non_full(taxa, produto)

    def recomendar_top_n(self, n=5):
        resultados = []
        self.root.reverse_traverse(resultados, n)
        return resultados
