cmake_minimum_required(VERSION 3.23)
project(aaaaaaaaaa C)

set(CMAKE_C_STANDARD 99)


# Définir _WIN32_WINNT pour utiliser une version plus récente de Windows
add_definitions(-D_WIN32_WINNT=0x0600)

# Ajouter cette ligne pour utiliser les fonctions inet_pton et inet_ntop
add_definitions(-DWIN32_LEAN_AND_MEAN)

if(WIN32)
    # Ajout des bibliothèques Winsock2
    link_libraries(ws2_32 wsock32)
endif()

add_executable(aaaaaaaaaa main.c server.c server.h client.c client.h game.h)

target_link_libraries(aaaaaaaaaa -lallegro-5.0.10-monolith-md-debug)
