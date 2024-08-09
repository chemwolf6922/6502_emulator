// @ts-check

function generateBinAsDecLut()
{
    let lut = new Array(256).fill(0xFF);
    for (let i = 0; i < 10; i++)
    {
        for (let j = 0; j < 10; j++)
        {
            lut[i * 0x10 + j] = i * 10 + j;
        }
    }
    return lut;
}

function generateDecAsBinLut()
{
    let lut = new Array(100);
    for (let i = 0; i < 100; i++)
    {
        lut[i] = (i / 10 << 4) | (i % 10);
    }
    return lut;
}

let output = '';
output += 'static const uint8_t DEC_AS_BIN[] =\n';
output += '{';
let lut = generateDecAsBinLut();
for (let i = 0; i < 100; i ++)
{
    if (i%10 == 0)
    {
        output += '\n    ';
    }
    output += `0x${lut[i].toString(16).padStart(2,'0')}, `;
}
output += '\n};\n';
console.log(output);

output = '';
output += 'static const uint8_t BIN_AS_DEC[] =\n';
output += '{';
lut = generateBinAsDecLut();
for (let i = 0; i < lut.length; i ++)
{
    if (i%16 == 0)
    {
        output += '\n    ';
    }
    output += `${lut[i]}, `.padStart(5,' ');
}
output += '\n};\n';
console.log(output);

