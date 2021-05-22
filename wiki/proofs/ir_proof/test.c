
int f = 55;

int myFunction(int f) {
    int x;
    int y = 8;
    int c = 100;

    x = y;
    y = f;
    f = c * (x + y) % 62;

    return f;
}

int main() {
    f = myFunction(f + 5);
    return f;
}
