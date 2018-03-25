function reloadURL()
{
  var url = location.protocol + '//' + location.host + location.pathname + "?" +
  (bitcoin_addresses.length > 0 ? "btc=" + bitcoin_addresses.join(",") : "");
  window.location.href = url;
}

function addBitcoin(address)
{
  bitcoin_addresses.push(address);
  reloadURL();
}

function deleteBitcoin(address)
{
  var index = bitcoin_addresses.indexOf(address);
  if (index > -1)
  {
    bitcoin_addresses.splice(index, 1);
    reloadURL();
  }
}
