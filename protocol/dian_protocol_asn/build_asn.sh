ASN=asn1c
DIRS=(src include)

$ASN dian.asn

function mkdirs(){
    for dir in ${DIRS[@]}; do
        echo create directory: $dir;
        mkdir -p $dir;
    done
}

function move(){
    mv *.c src/
    mv *.h include/
}

mkdirs
move