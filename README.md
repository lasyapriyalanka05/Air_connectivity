# ✈️ Dijkstra’s Algorithm in C Using Adjacency List (Graph of Places/Airports)

This C program implements **Dijkstra's Algorithm** using **Adjacency Lists** to find the shortest distance from a source place (e.g., airport or city) to all other places in a graph.

---

## 🧠 What It Does

- Takes user input for:
  - Number of places (vertices)
  - Names of the places
  - Connections between them (edges with distances)
- Represents the graph using adjacency lists
- Runs Dijkstra's algorithm from a **fixed source vertex (vertex 0)** (can be modified to make source dynamic)
- Outputs the **shortest distance** from the source place to all other places

---

## 📌 Sample Use Case

Imagine a scenario with 3 airports:

Dijkstra’s algorithm calculates the shortest distance from **Delhi** to each airport.

---

## 🛠️ How It Works

- **Graph Representation:** Adjacency List
- **Algorithm:** Dijkstra’s using Min Heap
- **Data Structures:**
  - `Graph`: Contains an array of adjacency lists
  - `MinHeap`: Supports efficient retrieval of minimum distance vertex

---

## 🧪 Sample Input

Enter number of places (vertices): 3
Place 1: Delhi
Place 2: Mumbai
Place 3: Chennai
Enter number of connections (edges): 2
Enter connections as:   
0 1 5
1 2 7

## ✅ Sample Output



---

## 💡 How to Run

### 1. Compile the Code

```bash
gcc dijkstra_places.c -o dijkstra

# RUN THE PROGRAM
./dijkstra

🧼 Memory Management

All dynamically allocated memory is properly freed using free() to avoid memory leaks.

⸻

🔧 Customization Ideas
	•	Change the source vertex dynamically via user input
	•	Display the actual shortest path (not just distance)
	•	Add support for reading from or writing to files
	•	Visualize the graph using external tools
👨‍💻 Author

Lasya Priya




