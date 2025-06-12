import re


input = open("../input.txt", "r").read().strip()
# print(input)

alphabet = "abcdefghijklmnopqrstuvxyz"
alphabet_map = {letter: n for n, letter in enumerate(alphabet)}
n_letters = len(alphabet)
increasing_straights = [".*" + alphabet[i : i + 3] + ".*" for i in range(n_letters - 2)]

# pp(alphabet_map)
# print(increasing_straights)


def get_digits(word):
    return [alphabet_map[a] for a in word]


def get_word(lista):
    return "".join([alphabet[i] for i in lista])


def to_base(number, base):
    """Converts a non-negative number to a list of digits in the given base.
    The base must be an integer greater than or equal to 2 and the first digit
    in the list of digits is the most significant one.
    """
    if not number:
        return [0]
    digits = []
    while number:
        digits.append(number % base)
        number //= base
    return list(reversed(digits))


def from_base(digits, base):
    """Converts a list of digits in the given base to an integer.
    The first digit is the most significant and the base is assumed to
    be an integer greater than or equal to 2.
    """
    power = 1
    number = 0
    for digit in reversed(digits):
        number += power * digit
        power *= base
    return number


def increment_word(word, n=1):
    lista = get_digits(word)
    number = from_base(lista, n_letters)
    new_number = number + n
    new_lista = to_base(new_number, n_letters)
    new_word = get_word(new_lista)
    return new_word


def check_conditions(word):
    cond_1 = re.match(r".*[i|o|l].*", word)
    cond_2 = re.match(r".*(\w)\1.*(\w)\2.*", word)
    cond_3 = set(
        filter(
            lambda x: x is not None, [re.match(p, word) for p in increasing_straights]
        )
    )
    if not cond_1 and cond_2 and cond_3:
        return True
    else:
        return False


def parte_1():
    word = input
    while True:
        if check_conditions(word):
            print(word)
            break
        word = increment_word(word)


def parte_2():
    word = input
    while True:
        if check_conditions(word):
            break
        word = increment_word(word)
    word = increment_word(word)
    while True:
        if check_conditions(word):
            print(word)
            break
        word = increment_word(word)


if __name__ == "__main__":
    parte_1()
    # parte_2()
