# Lumière Luxe
### Luxury Lighting & Chandeliers

Lumière Luxe is an interactive, console-based Brand-Affiliated E-Commerce Inventory and Search Engine System developed in C++. It demonstrates inventory management, product searching and sorting, shopping-cart operations, checkout processing, and administrative controls using the data structures and algorithms covered in Labs 1–8.

## Project Overview

The system simulates a luxury lighting e-commerce catalog featuring products from four official partner brands:

- Flos
- Bocci
- Tom Dixon
- Lasvit

The product catalog is organized into four categories:

- Chandeliers
- Pendant Lights
- Table Lamps
- Floor Lamps

All product prices are displayed in Philippine pesos (PHP).

## Main Features

| Menu | Feature | Description |
|---|---|---|
| 1 | View Complete Catalog | Displays all available products, prices, brands, categories, and stock quantities. |
| 2 | Filter Products by Official Brand | Displays products belonging to a selected partner brand. |
| 3 | Search Products by Keyword | Uses linear search to find products by name, category, or brand. It also offers a product-ID search using the BST. |
| 4 | Filter Products by Price Range | Uses binary search to find products within an inclusive minimum and maximum price range. |
| 5 | Shopping Cart | Adds and removes products and supports forward and backward cart navigation. |
| 6 | Customer Checkout | Creates customer orders and processes pending orders using FIFO order. |
| 7 | Admin Inventory Management | Updates prices and stock, adds and deletes products, and supports undo operations. |
| 8 | Partner Brand Showcase | Displays the four partner brands using a circular linked list. |
| 9 | Sort Inventory | Sorts products by price, stock quantity, or brand. |
| 10 | Exit | Closes the application. |

## Data Structures and Algorithms (Labs 1–8)

| Lab | Implementation | Purpose |
|---|---|---|
| Lab 1 | Dynamic Array | Stores the product catalog and automatically increases its capacity when needed. |
| Lab 2 | Doubly Linked List | Manages shopping-cart items and allows forward and backward navigation. |
| Lab 3 | Circular Linked List | Displays the official partner brands in a repeating cycle. |
| Lab 4 | Stack (LIFO) | Stores administrative actions so the latest action can be undone first. |
| Lab 5 | Queue (FIFO) | Stores and processes customer checkout orders in the order they are received. |
| Lab 6 | Binary Search Tree | Indexes product IDs for efficient product-ID lookup. |
| Lab 7 | Linear and Binary Search | Searches products by keyword and filters products within a price range. |
| Lab 8 | Insertion Sort and Quick Sort | Sorts products by price, brand, and stock quantity. |

## Requirements

- Windows operating system
- C++ compiler (such as MinGW-w64 through MSYS2)
- Windows Terminal, PowerShell, or another compatible console
- A C++ compiler that supports C++11 or later

The program uses the Windows API (`windows.h`) to enable ANSI terminal colors and the Windows `cls` command to clear the screen.

## How to Compile and Run

1. Save the source code as `main.cpp`.
2. Open PowerShell or the terminal in the folder containing `main.cpp`.
3. Compile the program:

   ```powershell
   g++ main.cpp -o main.exe
   ```

4. Run the application:

   ```powershell
   .\main.exe
   ```

If your compiler reports that `windows.h` cannot be found, make sure you are using a Windows-compatible MinGW compiler.

## Sample Product Catalog

The program starts with 16 sample products:

| ID | Product | Category | Brand | Price (PHP) | Stock |
|---:|---|---|---|---:|---:|
| 101 | Crystal Radiance | Chandeliers | Flos | 185,000.00 | 10 |
| 102 | Halo Pendant | Pendant Lights | Flos | 125,000.00 | 8 |
| 103 | Luna Table Lamp | Table Lamps | Flos | 85,000.00 | 12 |
| 104 | Arc Floor Lamp | Floor Lamps | Flos | 145,000.00 | 6 |
| 201 | Sculpted Glass | Chandeliers | Bocci | 350,000.00 | 4 |
| 202 | Orb Pendant | Pendant Lights | Bocci | 220,000.00 | 7 |
| 203 | Glass Table Light | Table Lamps | Bocci | 115,000.00 | 5 |
| 204 | Cascade Floor Light | Floor Lamps | Bocci | 275,000.00 | 3 |
| 301 | Melt Chandelier | Chandeliers | Tom Dixon | 320,000.00 | 3 |
| 302 | Melt Pendant | Pendant Lights | Tom Dixon | 165,000.00 | 9 |
| 303 | Bell Table Lamp | Table Lamps | Tom Dixon | 85,000.00 | 11 |
| 304 | Curve Floor Lamp | Floor Lamps | Tom Dixon | 195,000.00 | 5 |
| 401 | Crystal Cascade | Chandeliers | Lasvit | 450,000.00 | 2 |
| 402 | Bohemian Pendant | Pendant Lights | Lasvit | 285,000.00 | 4 |
| 403 | Glass Table Sculpture | Table Lamps | Lasvit | 135,000.00 | 6 |
| 404 | Prism Floor Light | Floor Lamps | Lasvit | 225,000.00 | 3 |

*Note: These are illustrative sample catalog entries and prices for the project. Product names should not be interpreted as verified official manufacturer model names.*

## Input Validation

The application checks user input for valid whole numbers, numeric prices, non-empty text, valid menu selections, and available product stock. The price-range search accepts values up to PHP 1,000,000,000,000 (one trillion).

## Important Notes

- Product and inventory changes are held in memory while the program is running.
- Changes are not saved to a database or external file.
- The initial catalog is reloaded whenever the application starts.
- The checkout queue processes orders in First-In, First-Out (FIFO) order.
- The administrative undo stack reverses the most recent supported admin action first (Last-In, First-Out or LIFO).
- The program uses dynamically allocated memory and releases owned memory through destructors.

## Project Structure

```text
Lumiere-Luxe/
├── main.cpp
└── README.md
```

## Project Purpose

Lumière Luxe demonstrates how fundamental C++ programming concepts, data structures, and algorithms can be combined into a practical console-based e-commerce inventory and search system. It is designed for academic demonstration of Labs 1–8.
