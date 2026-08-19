from PIL import Image
import sys

ICON_COLOR = (255, 255, 255)
BG_COLOR   =  (0, 0, 0)

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 convert.py <image.png> <size>")
        print("Example: python3 convert.py home_icon.png 24")
        sys.exit(1)

    input_file = sys.argv[1]
    size = int(sys.argv[2])
    output_file = input_file.rsplit(".", 1)[0] + ".h"
    array_name = input_file.rsplit(".", 1)[0].replace("-", "_")

    img = Image.open(input_file).convert("RGBA")
    img = img.resize((size, size), Image.LANCZOS)

    pixels = []
    for y in range(size):
        for x in range(size):
            r, g, b, a = img.getpixel((x, y))

            br, bg, bb = BG_COLOR
            ir, ig, ib = ICON_COLOR
            alpha = a / 255.0
            fr = int(ir * alpha + br * (1 - alpha))
            fg = int(ig * alpha + bg * (1 - alpha))
            fb = int(ib * alpha + bb * (1 - alpha))

            rgb565 = ((fr & 0xF8) << 8) | ((fg & 0xFC) << 3) | (fb >> 3)
            pixels.append(rgb565)

    with open(output_file, "w") as f:
        f.write("#pragma once\n")
        f.write("#include <pgmspace.h>\n\n")
        f.write(f"#define {array_name.upper()}_WIDTH  {size}\n")
        f.write(f"#define {array_name.upper()}_HEIGHT {size}\n\n")
        f.write(f"const uint16_t {array_name}[] PROGMEM = {{\n  ")
        hex_values = [f"0x{p:04X}" for p in pixels]
        f.write(",\n  ".join(
            ", ".join(hex_values[i:i+12]) for i in range(0, len(hex_values), 12)
        ))
        f.write("\n};\n")

    print(f"Input:  {input_file} (resized to {size}x{size})")
    print(f"Output: {output_file}")
    print(f"Array:  {array_name}[{len(pixels)}] = {len(pixels)*2} bytes")

if __name__ == "__main__":
    main()