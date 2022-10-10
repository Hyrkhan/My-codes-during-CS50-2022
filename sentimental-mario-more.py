# TODO
while True:
    try:
        height = int(input("Height: "))
    except ValueError:
        continue

    if height > 0 and height < 9:
        for i in range(0, height, 1):
            for j in range(height-1, i, -1):
                print(' ', end='')
            for k in range(0, i+1, 1):
                print('#', end='')
            print('  ', end='')
            for n in range(0, i+1, 1):
                print('#', end='')
            print("")
        break
