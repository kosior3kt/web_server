#/bin/bash

server_files=$(ls src/server)
server_src_files=$(echo "$server_files" | sed 's|^|src/server/|g')

client_files=$(ls src/client)
client_src_files=$(echo "$client_files" | sed 's|^|src/client/|g')

flags=(-g)
[[ "$1" == "-sanitize" ]] && flags+=(-fsanitize=address)

echo -e "\n\nSTART:\n\n"

gcc $server_src_files "${flags[@]}" -o build/server
#gcc $client_src_files "${flags}" -o build/client
