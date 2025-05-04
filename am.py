import pygame
import random
import string

# Initialize pygame
pygame.init()

# Screen settings
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Alphabet Mahjong Puzzle")

# Game settings
TILE_SIZE = 80
ROWS, COLS = 4, 6
GAP = 10
START_X = (WIDTH - (COLS * (TILE_SIZE + GAP))) // 2
START_Y = (HEIGHT - (ROWS * (TILE_SIZE + GAP))) // 2

# Colors
BG_COLOR = (245, 245, 245)
TILE_COLOR = (180, 180, 255)
REVEALED_COLOR = (255, 255, 255)
TEXT_COLOR = (0, 0, 0)
MATCH_COLOR = (180, 255, 180)

# Font
font = pygame.font.SysFont(None, 48)

# Generate letter pairs and shuffle
letters = random.sample(string.ascii_uppercase, ROWS * COLS // 2)
tiles_data = letters * 2
random.shuffle(tiles_data)

# Tile objects
tiles = []
for row in range(ROWS):
    for col in range(COLS):
        x = START_X + col * (TILE_SIZE + GAP)
        y = START_Y + row * (TILE_SIZE + GAP)
        letter = tiles_data.pop()
        tiles.append({
            'rect': pygame.Rect(x, y, TILE_SIZE, TILE_SIZE),
            'letter': letter,
            'shown': False,
            'matched': False
        })

# Game loop
selected = []
running = True
while running:
    screen.fill(BG_COLOR)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            for tile in tiles:
                if tile['rect'].collidepoint(pos) and not tile['shown'] and not tile['matched']:
                    tile['shown'] = True
                    selected.append(tile)

    if len(selected) == 2:
        pygame.time.delay(500)
        if selected[0]['letter'] == selected[1]['letter']:
            selected[0]['matched'] = True
            selected[1]['matched'] = True
        else:
            selected[0]['shown'] = False
            selected[1]['shown'] = False
        selected = []

    # Draw tiles
    for tile in tiles:
        color = MATCH_COLOR if tile['matched'] else (REVEALED_COLOR if tile['shown'] else TILE_COLOR)
        pygame.draw.rect(screen, color, tile['rect'])
        pygame.draw.rect(screen, (0, 0, 0), tile['rect'], 2)

        if tile['shown'] or tile['matched']:
            letter_surf = font.render(tile['letter'], True, TEXT_COLOR)
            letter_rect = letter_surf.get_rect(center=tile['rect'].center)
            screen.blit(letter_surf, letter_rect)

    pygame.display.flip()

pygame.quit()

