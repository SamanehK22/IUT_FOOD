# Class Diagrams

## Client-Side Class Structure

### Network Layer Classes

```
┌─────────────────────────────────────────────────────────────┐
│                    NetworkManager                           │
├─────────────────────────────────────────────────────────────┤
│ - tcpSocket: QTcpSocket*                                    │
│ - httpManager: QNetworkAccessManager*                       │
│ - serverUrl: QString                                        │
│ - serverPort: int                                           │
├─────────────────────────────────────────────────────────────┤
│ + connectToServer()                                         │
│ + sendTcpRequest(data: QJsonObject): void                   │
│ + sendRequest(endpoint: QString, data: QJsonObject): void   │
│ + disconnectFromServer()                                    │
│ - handleTcpResponse()                                       │
│ - handleHttpResponse()                                      │
└─────────────────────────────────────────────────────────────┘
```

### Manager Classes

```
┌─────────────────────────────────────────────────────────────┐
│                    AuthManager                              │
├─────────────────────────────────────────────────────────────┤
│ - networkManager: NetworkManager*                           │
│ - currentUser: User*                                        │
├─────────────────────────────────────────────────────────────┤
│ + login(email: QString, password: QString): void           │
│ + register(userData: QJsonObject): void                    │
│ + logout(): void                                           │
│ + getCurrentUser(): User*                                  │
│ - handleLoginResponse()                                    │
│ - handleRegisterResponse()                                 │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                  CustomerManager                            │
├─────────────────────────────────────────────────────────────┤
│ - networkManager: NetworkManager*                           │
│ - cart: Cart*                                              │
├─────────────────────────────────────────────────────────────┤
│ + getRestaurants(): void                                   │
│ + getMenu(restaurantId: int): void                         │
│ + placeOrder(orderData: QJsonObject): void                │
│ + getOrderHistory(): void                                  │
│ + addToCart(item: MenuItem): void                          │
│ + removeFromCart(itemId: QString): void                    │
│ - handleRestaurantsResponse()                              │
│ - handleMenuResponse()                                     │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│               RestaurantOwnerManager                        │
├─────────────────────────────────────────────────────────────┤
│ - networkManager: NetworkManager*                           │
│ - currentRestaurant: Restaurant*                           │
├─────────────────────────────────────────────────────────────┤
│ + getOrders(): void                                        │
│ + updateOrderStatus(orderId: int, status: QString): void   │
│ + manageMenu(): void                                       │
│ + addMenuItem(item: MenuItem): void                        │
│ + updateMenuItem(item: MenuItem): void                     │
│ + deleteMenuItem(itemId: QString): void                    │
│ - handleOrdersResponse()                                   │
│ - handleMenuUpdateResponse()                               │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    AdminManager                             │
├─────────────────────────────────────────────────────────────┤
│ - networkManager: NetworkManager*                           │
│ - users: QList<User*>                                      │
│ - restaurants: QList<Restaurant*>                          │
├─────────────────────────────────────────────────────────────┤
│ + getUsers(): void                                         │
│ + getRestaurants(): void                                   │
│ + approveRestaurant(restaurantId: int): void               │
│ + rejectRestaurant(restaurantId: int): void                │
│ + deleteUser(userId: int): void                            │
│ + getStatistics(): void                                    │
│ - handleUsersResponse()                                    │
│ - handleRestaurantsResponse()                              │
└─────────────────────────────────────────────────────────────┘
```

### Model Classes

```
┌─────────────────────────────────────────────────────────────┐
│                        User                                 │
├─────────────────────────────────────────────────────────────┤
│ - id: int                                                   │
│ - username: QString                                         │
│ - email: QString                                            │
│ - userType: QString                                         │
│ - isActive: bool                                            │
│ - createdAt: QDateTime                                      │
├─────────────────────────────────────────────────────────────┤
│ + getId(): int                                             │
│ + getUsername(): QString                                   │
│ + getEmail(): QString                                      │
│ + getUserType(): QString                                   │
│ + isActive(): bool                                         │
│ + getCreatedAt(): QDateTime                                │
│ + setId(id: int): void                                     │
│ + setUsername(username: QString): void                     │
│ + setEmail(email: QString): void                           │
│ + setUserType(type: QString): void                         │
│ + setActive(active: bool): void                            │
│ + toJson(): QJsonObject                                    │
│ + fromJson(json: QJsonObject): void                        │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                     Restaurant                              │
├─────────────────────────────────────────────────────────────┤
│ - id: int                                                   │
│ - name: QString                                             │
│ - address: QString                                          │
│ - phone: QString                                            │
│ - ownerId: int                                              │
│ - isApproved: bool                                          │
│ - rating: double                                            │
│ - createdAt: QDateTime                                      │
├─────────────────────────────────────────────────────────────┤
│ + getId(): int                                             │
│ + getName(): QString                                       │
│ + getAddress(): QString                                    │
│ + getPhone(): QString                                      │
│ + getOwnerId(): int                                        │
│ + isApproved(): bool                                       │
│ + getRating(): double                                      │
│ + getCreatedAt(): QDateTime                                │
│ + setName(name: QString): void                             │
│ + setAddress(address: QString): void                       │
│ + setPhone(phone: QString): void                           │
│ + setApproved(approved: bool): void                        │
│ + setRating(rating: double): void                          │
│ + toJson(): QJsonObject                                    │
│ + fromJson(json: QJsonObject): void                        │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                      MenuItem                               │
├─────────────────────────────────────────────────────────────┤
│ - id: QString                                               │
│ - name: QString                                             │
│ - description: QString                                      │
│ - price: double                                             │
│ - category: QString                                         │
│ - imageUrl: QString                                         │
│ - available: bool                                           │
│ - ingredients: QString                                      │
├─────────────────────────────────────────────────────────────┤
│ + getId(): QString                                         │
│ + getName(): QString                                       │
│ + getDescription(): QString                                │
│ + getPrice(): double                                       │
│ + getCategory(): QString                                   │
│ + getImageUrl(): QString                                   │
│ + isAvailable(): bool                                      │
│ + getIngredients(): QString                                │
│ + setId(id: QString): void                                 │
│ + setName(name: QString): void                             │
│ + setDescription(desc: QString): void                      │
│ + setPrice(price: double): void                            │
│ + setCategory(category: QString): void                     │
│ + setImageUrl(url: QString): void                          │
│ + setAvailable(available: bool): void                      │
│ + setIngredients(ingredients: QString): void               │
│ + toJson(): QJsonObject                                    │
│ + fromJson(json: QJsonObject): void                        │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                        Order                                │
├─────────────────────────────────────────────────────────────┤
│ - id: int                                                   │
│ - customerId: int                                           │
│ - restaurantId: int                                         │
│ - totalAmount: double                                       │
│ - status: QString                                           │
│ - items: QList<OrderItem>                                   │
│ - createdAt: QDateTime                                      │
│ - updatedAt: QDateTime                                      │
├─────────────────────────────────────────────────────────────┤
│ + getId(): int                                             │
│ + getCustomerId(): int                                     │
│ + getRestaurantId(): int                                   │
│ + getTotalAmount(): double                                 │
│ + getStatus(): QString                                     │
│ + getItems(): QList<OrderItem>                             │
│ + getCreatedAt(): QDateTime                                │
│ + getUpdatedAt(): QDateTime                                │
│ + setStatus(status: QString): void                         │
│ + addItem(item: OrderItem): void                           │
│ + removeItem(itemId: QString): void                        │
│ + calculateTotal(): void                                   │
│ + toJson(): QJsonObject                                    │
│ + fromJson(json: QJsonObject): void                        │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                      Cart                                   │
├─────────────────────────────────────────────────────────────┤
│ - items: QList<CartItem>                                    │
│ - totalAmount: double                                       │
├─────────────────────────────────────────────────────────────┤
│ + getItems(): QList<CartItem>                              │
│ + getTotalAmount(): double                                 │
│ + addItem(item: MenuItem, quantity: int): void             │
│ + removeItem(itemId: QString): void                        │
│ + updateQuantity(itemId: QString, quantity: int): void     │
│ + clear(): void                                            │
│ + calculateTotal(): void                                   │
│ + toJson(): QJsonObject                                    │
│ + fromJson(json: QJsonObject): void                        │
└─────────────────────────────────────────────────────────────┘
```

## Server-Side Class Structure

### Network Layer Classes

```
┌─────────────────────────────────────────────────────────────┐
│                      Server                                 │
├─────────────────────────────────────────────────────────────┤
│ - tcpServer: QTcpServer*                                    │
│ - httpServer: QHttpServer*                                  │
│ - databaseManager: DatabaseManager*                         │
│ - connectedClients: QList<QTcpSocket*>                      │
├─────────────────────────────────────────────────────────────┤
│ + start(): bool                                            │
│ + stop(): void                                             │
│ + handleNewConnection(): void                              │
│ + handleClientDisconnection(): void                        │
│ + broadcastMessage(message: QJsonObject): void             │
│ - handleTcpRequest(): void                                 │
│ - handleHttpRequest(): void                                │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                 WebSocketServer                             │
├─────────────────────────────────────────────────────────────┤
│ - server: QWebSocketServer*                                 │
│ - clients: QList<QWebSocket*>                               │
├─────────────────────────────────────────────────────────────┤
│ + start(port: int): bool                                   │
│ + stop(): void                                             │
│ + broadcastMessage(message: QJsonObject): void             │
│ - handleNewConnection(): void                              │
│ - handleClientDisconnection(): void                        │
│ - handleMessage(message: QString): void                    │
└─────────────────────────────────────────────────────────────┘
```

### Business Logic Classes

```
┌─────────────────────────────────────────────────────────────┐
│                 DatabaseManager                             │
├─────────────────────────────────────────────────────────────┤
│ - database: QSqlDatabase                                    │
│ - connectionName: QString                                   │
├─────────────────────────────────────────────────────────────┤
│ + connect(): bool                                          │
│ + disconnect(): void                                       │
│ + executeQuery(query: QString): QSqlQuery                 │
│ + executeTransaction(queries: QStringList): bool           │
│ + getUserById(id: int): User*                              │
│ + getRestaurantById(id: int): Restaurant*                  │
│ + getMenuByRestaurantId(id: int): QList<MenuItem*>         │
│ + createOrder(order: Order): bool                          │
│ + updateOrderStatus(id: int, status: QString): bool        │
│ + getOrdersByCustomerId(id: int): QList<Order*>            │
│ + getOrdersByRestaurantId(id: int): QList<Order*>          │
│ - initializeTables(): void                                 │
│ - createUserTable(): void                                  │
│ - createRestaurantTable(): void                            │
│ - createMenuItemTable(): void                              │
│ - createOrderTable(): void                                 │
│ - createOrderItemTable(): void                             │
│ - createChatMessageTable(): void                           │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                 AuthSystem                                  │
├─────────────────────────────────────────────────────────────┤
│ - databaseManager: DatabaseManager*                         │
├─────────────────────────────────────────────────────────────┤
│ + authenticateUser(email: QString, password: QString): User*│
│ + registerUser(userData: QJsonObject): bool                │
│ + validateToken(token: QString): bool                      │
│ + generateToken(user: User): QString                       │
│ + hashPassword(password: QString): QString                 │
│ + verifyPassword(password: QString, hash: QString): bool   │
│ - validateEmail(email: QString): bool                      │
│ - validatePassword(password: QString): bool                │
└─────────────────────────────────────────────────────────────┘
```

### Command Pattern Classes

```
┌─────────────────────────────────────────────────────────────┐
│                      Command                                │
├─────────────────────────────────────────────────────────────┤
│ + execute(): void                                          │
│ + undo(): void                                            │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                   OrderCommand                              │
├─────────────────────────────────────────────────────────────┤
│ - order: Order*                                            │
│ - databaseManager: DatabaseManager*                         │
│ - orderManager: OrderManager*                               │
├─────────────────────────────────────────────────────────────┤
│ + execute(): void                                          │
│ + undo(): void                                            │
│ + getOrder(): Order*                                       │
│ + setOrder(order: Order*): void                            │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                      Invoker                                │
├─────────────────────────────────────────────────────────────┤
│ - commands: QList<Command*>                                 │
│ - currentIndex: int                                         │
├─────────────────────────────────────────────────────────────┤
│ + executeCommand(command: Command*): void                  │
│ + undo(): void                                            │
│ + redo(): void                                            │
│ + canUndo(): bool                                         │
│ + canRedo(): bool                                         │
│ + clearHistory(): void                                     │
└─────────────────────────────────────────────────────────────┘
```

### Observer Pattern Classes

```
┌─────────────────────────────────────────────────────────────┐
│                     Observer                                │
├─────────────────────────────────────────────────────────────┤
│ + update(subject: Subject*): void                          │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                     Subject                                 │
├─────────────────────────────────────────────────────────────┤
│ - observers: QList<Observer*>                               │
├─────────────────────────────────────────────────────────────┤
│ + attach(observer: Observer*): void                        │
│ + detach(observer: Observer*): void                        │
│ + notify(): void                                           │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                OrderStatusObserver                          │
├─────────────────────────────────────────────────────────────┤
│ - order: Order*                                            │
│ - webSocketServer: WebSocketServer*                         │
├─────────────────────────────────────────────────────────────┤
│ + update(subject: Subject*): void                          │
│ + notifyCustomer(): void                                   │
│ + notifyRestaurant(): void                                 │
└─────────────────────────────────────────────────────────────┘
```

## Class Relationships

### Inheritance Relationships
- `OrderCommand` inherits from `Command`
- `OrderStatusObserver` inherits from `Observer`
- `Order` implements `Subject` interface

### Composition Relationships
- `Server` contains `DatabaseManager`, `WebSocketServer`
- `NetworkManager` contains `QTcpSocket`, `QNetworkAccessManager`
- `Cart` contains `QList<CartItem>`
- `Order` contains `QList<OrderItem>`

### Association Relationships
- `AuthManager` uses `NetworkManager`
- `CustomerManager` uses `NetworkManager`
- `RestaurantOwnerManager` uses `NetworkManager`
- `AdminManager` uses `NetworkManager`
- `OrderCommand` uses `DatabaseManager`, `OrderManager`
- `OrderStatusObserver` uses `WebSocketServer`

### Dependency Relationships
- All manager classes depend on `NetworkManager`
- All model classes depend on `QJsonObject` for serialization
- UI classes depend on their respective manager classes 