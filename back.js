// Arrays de mapeamento dos rotores
const vogaisTrocada1 = [5, 20, 19, 8, 7, 0, 21, 4, 3, 23, 11, 10, 25, 24, 15, 14, 22, 18, 17, 2, 1, 6, 16, 9, 13, 12];
const vogaisTrocada2 = [20, 8, 11, 18, 12, 22, 23, 21, 1, 24, 17, 2, 4, 15, 25, 13, 19, 10, 3, 16, 0, 7, 5, 6, 9, 14];
const vogaisTrocada3 = [9, 2, 1, 17, 22, 24, 16, 21, 15, 0, 23, 20, 14, 19, 12, 8, 6, 3, 25, 13, 11, 7, 4, 10, 5, 18];
const vogaisTrocadaRefletor = [9, 11, 8, 22, 14, 7, 21, 5, 2, 0, 20, 1, 17, 16, 4, 25, 13, 12, 23, 24, 10, 6, 3, 18, 19, 15];

const alphaValues = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
                     'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'];

// Função para rotacionar os arrays
function rotacionaArray(arr, size) {
    let temp = arr[0];
    for (let i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    arr[size - 1] = temp;
}

// Função para codificar a letra
function codificarLetra(letra, rotorPosicao1, rotorPosicao2, rotorPosicao3) {
    const index = alphaValues.indexOf(letra.toUpperCase());
    if (index === -1) return 'Letra inválida';

    let posTrocada1 = (index + rotorPosicao1) % 26;
    let posTrocada2 = vogaisTrocada1[posTrocada1];
    let posTrocada3 = vogaisTrocada2[posTrocada2];
    let posTrocadaRefletor = vogaisTrocada3[posTrocada3];
    let letraRefletor = vogaisTrocadaRefletor[posTrocadaRefletor];

    let posFinal3, posFinal2, posFinal1;
    for (let i = 0; i < 26; i++) {
        if (vogaisTrocada3[i] == letraRefletor) {
            posFinal3 = i;
        }
    }
    for (let i = 0; i < 26; i++) {
        if (vogaisTrocada2[i] == posFinal3) {
            posFinal2 = i;
        }
    }
    for (let i = 0; i < 26; i++) {
        if (vogaisTrocada1[i] == posFinal2) {
            posFinal1 = i;
        }
    }

    // Retorna a letra codificada
    return alphaValues[posFinal1];
}

// Função que é chamada quando o botão "Codificar" é pressionado
function codificar() {
    const letra = document.getElementById('letra').value;
    let rotorPosicao1 = parseInt(document.getElementById('rotorPosicao1').value);
    let rotorPosicao2 = parseInt(document.getElementById('rotorPosicao2').value);
    let rotorPosicao3 = parseInt(document.getElementById('rotorPosicao3').value);

    if (!letra || rotorPosicao1 < 0 || rotorPosicao2 < 0 || rotorPosicao3 < 0 ||
        rotorPosicao1 > 25 || rotorPosicao2 > 25 || rotorPosicao3 > 25) {
        alert("Por favor, insira uma letra válida e posições dos rotores entre 0 e 25.");
        return;
    }

    // Codificar a letra
    const letraCodificada = codificarLetra(letra, rotorPosicao1, rotorPosicao2, rotorPosicao3);

    // Exibir o resultado
    document.getElementById('resultado').textContent = `Letra codificada: ${letraCodificada}`;

    // Rotação dos rotores
    if (rotorPosicao1 < 25) {
        rotacionaArray(vogaisTrocada1, 26);
        rotorPosicao1++;
    } else {
        rotorPosicao1 = 0;
        rotacionaArray(vogaisTrocada1, 26);
        rotacionaArray(vogaisTrocada2, 26);
        rotorPosicao2++;

        if (rotorPosicao2 == 26) {
            rotorPosicao2 = 0;
            rotacionaArray(vogaisTrocada1, 26);
            rotacionaArray(vogaisTrocada2, 26);
            rotacionaArray(vogaisTrocada3, 26);
            rotorPosicao3++;
        }
    }
}