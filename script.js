function updateParkingStatus() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/parking_status", true);
    xhr.onreadystatechange = function() {
      if (xhr.readyState == 4 && xhr.status == 200) {
        var parkingStatus = xhr.responseText.split("\n");
        var parkingStatusDiv = document.getElementById("parkingStatus");
        parkingStatusDiv.innerHTML = ""; // Clear previous content
        for (var i = 0; i < parkingStatus.length; i++) {
          var slotStatus = parkingStatus[i].split(":");
          var slotDiv = document.createElement("div");
          slotDiv.textContent = slotStatus[0] + ": " + slotStatus[1];
          if (slotStatus[1].trim() == "Booked") {
            slotDiv.style.color = "red"; // Set color for booked slots
          }
          parkingStatusDiv.appendChild(slotDiv);
        }
      }
    };
    xhr.send();
  }
  
  function bookSlot() {
    var vehicleNumber = document.getElementById("vehicleNumber").value;
    var ownerName = document.getElementById("ownerName").value;
    var vehicleName = document.getElementById("vehicleName").value;
    var entryDate = document.getElementById("entryDate").value;
    var exitDate = document.getElementById("exitDate").value;
    var slotNumber = document.getElementById("slotNumber").value;
    
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/book_slot?slotNumber=" + slotNumber, true);
    xhr.onreadystatechange = function() {
      if (xhr.readyState == 4) {
        alert(xhr.responseText);
        updateParkingStatus();
      }
    };
    xhr.send();
  }
  
  // Update parking status on page load
  updateParkingStatus();
  