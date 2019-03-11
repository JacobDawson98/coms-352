from random import randint
from math import sqrt


def is_square(num):
    return sqrt(num).is_integer()


open('input.txt', 'w').close()
with open('input.txt', 'a') as f:
    num_rows_cols = randint(0, 300)
    while not is_square(num_rows_cols):
        num_rows_cols = randint(0, 300)
    for num in range(num_rows_cols):
        randy = randint(1, 100)
        print(randy)
        f.write(str(randy) + ' ')
    print('Number of ints entered:', num_rows_cols)
