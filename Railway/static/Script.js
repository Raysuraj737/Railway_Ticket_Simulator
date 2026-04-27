function addPassenger() {
  const container = document.getElementById("passengerContainer");

  const div = document.createElement("div");
  div.className = "passenger-block";

  div.innerHTML = `
    <div class="passenger-header">
      <h3></h3>
      <button type="button" class="remove" onclick="removePassenger(this)">✖</button>
    </div>

    <div class="field">
      <i class="fa fa-user"></i>
      <input type="text" name="name[]" required placeholder=" " />
      <label>Name</label>
    </div>

    <div class="field">
      <i class="fa fa-birthday-cake"></i>
      <input type="number" name="age[]" required placeholder=" " />
      <label>Age</label>
    </div>

    <div class="field">
      <i class="fa fa-train"></i>
      <input type="text" name="src[]" required placeholder=" " />
      <label>Source</label>
    </div>

    <div class="field">
      <i class="fa fa-map-marker"></i>
      <input type="text" name="dest[]" required placeholder=" " />
      <label>Destination</label>
    </div>

    <div class="field">
      <i class="fa fa-bed"></i>
      <input list="levels" name="berth[]" required placeholder=" " />
      <label>Berth</label>
    </div>
  `;

  container.appendChild(div);
  updatePassengerNumbers();
}

function removePassenger(btn) {
  btn.closest(".passenger-block").remove();
  updatePassengerNumbers();
}

function updatePassengerNumbers() {
  const passengers = document.querySelectorAll(".passenger-block h3");
  passengers.forEach((h3, index) => {
    h3.textContent = `Passenger ${index + 1}`;
  });
}

function closePopup() {
  const popup = document.getElementById("popup");
  if (popup) popup.style.display = "none";
}

function printTicket() {
  window.print();
}

function downloadPDF() {
  if (typeof ticketData === "undefined") {
    alert("Ticket data not available");
    return;
  }

  const printArea = document.getElementById("print-area");

  const opt = {
    margin: 0.3,
    filename: `${ticketData.ticket_no}.pdf`,
    image: { type: "jpeg", quality: 0.98 },
    html2canvas: { scale: 2 },
    jsPDF: { unit: "in", format: "letter", orientation: "portrait" }
  };

  html2pdf().set(opt).from(printArea).save();
}
