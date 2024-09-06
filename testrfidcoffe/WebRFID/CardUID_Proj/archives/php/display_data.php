<?php
$hostname = "localhost";
$username = "root";
$password = ":698yP9y?)*xVYK";
$database = "rfidtag";

// Create connection
$conn = new mysqli($hostname, $username, $password, $database);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Retrieve data
$sql = "SELECT CardUID, Name FROM rfidrc522";
$result = $conn->query($sql);

?>

<!DOCTYPE html>
<html>
<head>
    <title>RFID Tag Database</title>
    <style>
        table {
            width: 50%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black;
        }
        th, td {
            padding: 15px;
            text-align: left;
        }
    </style>
</head>
<body>

<h1>RFID Tag Database</h1>

<?php
if ($result->num_rows > 0) {
    echo "<table><tr><th>CardUID</th><th>Name</th></tr>";
    // Output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<tr><td>" . $row["CardUID"]. "</td><td>" . $row["Name"]. "</td></tr>";
    }
    echo "</table>";
} else {
    echo "0 results";
}
$conn->close();
?>

</body>
</html>
