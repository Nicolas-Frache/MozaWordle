


def is_valid_password(uc_password: str):
    # Un password est valide ssi il comporte entre 10 et 32 carractères, dont 1 chiffre, 1 minuscule et 1 majuscule
    message = ""
    longueur = True
    if len(uc_password) < 10 or len(uc_password) > 32:
        longueur = False
        message += "Le mot de passe doit être entre 10 et 32 caractères. "
    maj, min, num = False, False, False

    for c in uc_password:
        maj = maj + c.isupper()
        min = min + c.islower()
        num = num + c.isdigit()
    
    if not maj:
        message += " Il faut au moins une majuscule. "
    if not min:
        message += " Il faut au moins une minuscule. "    
    if not num:
        message += " Il faut au moins un chiffre "

    return bool(maj * min * num * longueur),message


def encrypt(uc_string: str) -> str:
    ec_string = uc_string
    # TODO encypter
    return ec_string


def tri_fusion(tab: list) -> None:  # tri à gauche
    # tri fusion sur tab
    if len(tab) > 1:
        mid = len(tab) // 2

        G = tab[:mid]  # sous-tableau gauche
        D = tab[mid:]  # sous-tableau droit

        tri_fusion(G)
        tri_fusion(D)

        # Fusion
        i = 0
        j = 0
        k = 0

        while i < len(G) and j < len(D):
            if G[i][1] > D[j][1]:
                tab[k] = G[i]
                i += 1
            else:
                tab[k] = D[j]
                j += 1
            k += 1

        while i < len(G):
            tab[k] = G[i]
            i += 1
            k += 1

        while j < len(D):
            tab[k] = D[j]
            j += 1
            k += 1
    return None


def index_of_custom(tab: list, element: tuple) -> int:  # tab.index(element) mais qui return 0 en cas d' error
    try:
        return tab.index(element) + 1
    except ValueError:
        return 0


def most_common_in_list(tab: list, pref_type: str) -> int: # Optimisable
    L = []
    a = 0
    b = 0
    if pref_type == "difficulty":
        b = 3
    elif pref_type == "longueur":
        a = 2
        b = 15
    for i in range(a, b):
        L.append(tab.count(i+1))
    maxi = max(L)
    if maxi != 0:
        for i in range(b-a):
            if L[i] == maxi:
                return i+1+a
    return 0
