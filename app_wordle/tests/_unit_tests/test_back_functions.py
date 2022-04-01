import fonctions
from database.dict_tools import get_random_word


def test_Valid_Password():
    invalids_pwd_list = ["abcdefghijk",
                         "Abc1",
                         "123",
                         "ABCdefghijkl",
                         "Abc1xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                         "",
                         "abcdefghijkl123"]
    valids_pwd_list = ["Abcdefghijkl123",
                       "AAAAAAAAAAAAAa69",
                       "dlm69x420x69xD",
                       "RBHJZEHJZEKNRGKJGRZJEFBHREBFHZb4",
                       "StrongPwd1"]

    for pwd in invalids_pwd_list:
        assert not fonctions.ValidPassword(pwd)

    for pwd in valids_pwd_list:
        assert fonctions.ValidPassword(pwd)


# TODO: régler le problème lors de ce test (ModuleNotFoundError: No module named 'database')
'''
def test_get_random_word():
    for length in range(3, 25):
        assert len(get_random_word(length)) == length
'''
