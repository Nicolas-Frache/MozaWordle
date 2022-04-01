
def ValidPassword(uc_password : str) -> bool:
    # Un password est valide ssi il comporte entre 10 et 32 carractères, dont 1 chiffre, 1 minuscule et 1 majuscule
    if len(uc_password) < 10 or len(uc_password) > 32:
        return False

    maj, min, num = False, False, False

    for c in uc_password:
        maj = maj + c.isupper()
        min = min + c.islower()
        num = num + c.isdigit()

    return bool(maj * min * num)


def Encrypt(uc_string : str) -> str:

    ec_string = uc_string

    #TODO encypter

    return ec_string