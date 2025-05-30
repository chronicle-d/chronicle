import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { Loader2, Settings, HardDrive, ChevronDown, Home } from 'lucide-react';
import Head from 'next/head';

const API_BASE = 'http://127.0.0.1:5000';

export default function ChronicleDashboard() {
  const [devices, setDevices] = useState([]);
  const [featured, setFeatured] = useState([]);
  const [selectedDevice, setSelectedDevice] = useState(null);
  const [deviceSettings, setDeviceSettings] = useState(null);
  const [deviceConfig, setDeviceConfig] = useState(null);
  const [settings, setSettings] = useState(null);
  const [showSettings, setShowSettings] = useState(false);
  const [dropdownOpen, setDropdownOpen] = useState(false);
  const [errors, setErrors] = useState([]);
  const [loading, setLoading] = useState(false);
  const [sidebarCollapsed, setSidebarCollapsed] = useState(false);

  useEffect(() => {
    fetchDevices();
    fetchChronicleSettings();
  }, []);

  const showError = (input) => {
    if (!input) return;
    const responseData = input?.response?.data;
    const err = responseData?.data?.error;
    const code = err?.code;
    const codeMessage = err?.codeMessage;
    const details = err?.details;

    let content = '';
    if (code && codeMessage) {
      content += `Error ${code}: ${codeMessage}`;
      if (details) content += `\n\n${details}`;
    } else if (input?.response?.status) {
      content = `Request failed with status ${input.response.status}`;
    } else if (input?.message) {
      content = input.message;
    } else {
      content = typeof input === 'string' ? input : JSON.stringify(input, null, 2);
    }

    const id = Date.now();
    setErrors((prev) => [...prev, { id, content }]);
    setTimeout(() => {
      setErrors((prev) => prev.filter((e) => e.id !== id));
    }, 5000);
  };

  const fetchDevices = async () => {
    try {
      const res = await axios.get(`${API_BASE}/listDevices`);
      if (res.data.success) {
        setDevices(res.data.data.devices);
        const featuredData = await Promise.all(
          res.data.data.devices.slice(0, 2).map(async (name) => {
            try {
              const detailRes = await axios.get(`${API_BASE}/getDeviceSettings?name=${name}`);
              return detailRes.data.data;
            } catch {
              return null;
            }
          })
        );
        setFeatured(featuredData.filter(Boolean));
      } else {
        showError(res);
      }
    } catch (err) {
      showError(err);
    }
  };

  const fetchChronicleSettings = async () => {
    try {
      const res = await axios.get(`${API_BASE}/getSettings`);
      if (res.data.success) setSettings(res.data.data);
    } catch (err) {
      showError(err);
    }
  };

  const selectDevice = async (device) => {
    setShowSettings(false);
    setSelectedDevice(device);
    setDeviceSettings(null);
    setDeviceConfig(null);
    setDropdownOpen(false);
    try {
      const res = await axios.get(`${API_BASE}/getDeviceSettings?name=${device}`);
      if (res.data.success) setDeviceSettings(res.data.data);
      else showError(res);
    } catch (err) {
      showError(err);
    }
  };

  const fetchDeviceConfig = async () => {
    setDeviceConfig(null);
    setLoading(true);
    try {
      const res = await axios.get(`${API_BASE}/getDeviceConfig?name=${selectedDevice}`);
      if (res.data.success) setDeviceConfig(res.data.data);
      else showError(res);
    } catch (err) {
      showError(err);
    }
    setLoading(false);
  };

  return (
    <div className="flex min-h-screen bg-white text-gray-800">
      <Head>
        <title>Chronicle UI</title>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
      </Head>

      <aside className={`${sidebarCollapsed ? 'w-[56px]' : 'w-64'} fixed top-0 left-0 z-40 h-full transition-all duration-300 bg-gray-900 text-white p-3 flex flex-col space-y-4 shadow-lg`}>
        <div className="flex items-center justify-between">
          <h1 className={`text-2xl font-bold tracking-tight mb-6 ${sidebarCollapsed ? 'hidden' : 'block'}`}>Chronicle</h1>
          <button onClick={() => setSidebarCollapsed(!sidebarCollapsed)} className="text-white focus:outline-none ml-auto">
            {sidebarCollapsed ? '»' : '«'}
          </button>
        </div>
        <button className="text-left px-3 py-2 text-white hover:bg-gray-800 bg-gray-700 rounded" onClick={() => { setShowSettings(false); setSelectedDevice(null); }}>
          <Home className="inline mr-2" size={18} /> {!sidebarCollapsed && 'Home'}
        </button>
        <button className="text-left px-3 py-2 text-white hover:bg-gray-800 bg-gray-700 rounded" onClick={() => { setShowSettings(true); setSelectedDevice(null); setDeviceConfig(null); }}>
          <Settings className="inline mr-2" size={18} /> {!sidebarCollapsed && 'Settings'}
        </button>
        <div className="relative">
          <button className="w-full text-left px-3 py-2 text-white hover:bg-gray-800 bg-gray-700 rounded" onClick={() => setDropdownOpen(!dropdownOpen)}>
            <HardDrive className="inline mr-2" size={18} /> {!sidebarCollapsed && 'Devices'} <ChevronDown className="inline ml-auto" size={16} />
          </button>
          {dropdownOpen && (
            <div className={`absolute left-0 top-full mt-2 bg-gray-800 border border-gray-700 rounded text-sm z-50 min-w-full ${sidebarCollapsed ? 'ml-2' : ''}`}>
              <ul>
                {devices.map((d) => (
                  <li key={d}>
                    <button onClick={() => selectDevice(d)} className="w-full text-left px-4 py-2 hover:bg-gray-700">
                      {d}
                    </button>
                  </li>
                ))}
              </ul>
            </div>
          )}
        </div>
      </aside>

      <main className={`flex-1 overflow-y-auto transition-all duration-300 ${sidebarCollapsed ? 'ml-[56px]' : 'ml-64'} flex justify-center items-start px-8 py-12`}>
        <div className="w-full max-w-4xl">
          {errors.map((err) => (
            <div key={err.id} className="fixed top-4 right-4 z-50 bg-red-500/90 text-white px-6 py-4 rounded shadow-lg text-sm font-mono whitespace-pre-wrap border border-red-700 w-[420px] max-w-[calc(100vw-2rem)]">
              <div className="flex justify-between items-start">
                <div className="flex gap-2">
                  <span className="text-xl mt-0.5">⚠️</span>
                  <div className="text-left">
                    {err.content.split('\n').map((line, i) => (
                      <div key={i} className={i === 0 ? 'font-semibold text-base mb-1' : 'text-white/90'}>{line}</div>
                    ))}
                  </div>
                </div>
                <button onClick={() => setErrors(errors.filter((e) => e.id !== err.id))} className="ml-2 text-white font-bold">×</button>
              </div>
            </div>
          ))}

          {showSettings && settings && (
            <div className="mt-4">
              <h2 className="text-2xl font-semibold mb-4">Chronicle Settings</h2>
              <div className="bg-gray-100 border p-4 rounded text-sm space-y-2">
                <div><strong>Idle Timeout:</strong> {settings.ssh.sshIdleTimeout} ms</div>
                <div><strong>Total Timeout:</strong> {settings.ssh.sshTotalTimeout} ms</div>
              </div>
            </div>
          )}

          {selectedDevice && deviceSettings && (
            <div className="mt-6">
              <h2 className="text-2xl font-semibold mb-4">Device: {selectedDevice}</h2>
              <div className="bg-gray-50 border p-4 rounded space-y-2 text-sm">
                <div><strong>Device:</strong> {deviceSettings.device.deviceName} ({deviceSettings.device.vendorName})</div>
                <div><strong>Host:</strong> {deviceSettings.ssh.host}</div>
                <div><strong>User:</strong> {deviceSettings.ssh.user}</div>
                <div><strong>Port:</strong> {deviceSettings.ssh.port}</div>
              </div>
              <div className="mt-4 flex justify-center">
                <button onClick={fetchDeviceConfig} disabled={loading} className="px-6 py-2 bg-gray-800 text-white hover:bg-gray-700 rounded">
                  {loading ? (
                    <>
                      <Loader2 className="animate-spin h-5 w-5 text-white mr-2" />
                      Fetching...
                    </>
                  ) : 'Fetch Configuration'}
                </button>
              </div>
              {deviceConfig && (
                <div className="mt-4 max-h-[400px] overflow-y-auto bg-gray-900 text-green-400 font-mono p-4 rounded">
                  {deviceConfig.config.map((line, idx) => (<div key={idx}>{line}</div>))}
                </div>
              )}
            </div>
          )}

          {!showSettings && !selectedDevice && featured.length > 0 && (
            <div>
              <h2 className="text-2xl font-semibold mb-4">Featured Devices</h2>
              <div className="grid grid-cols-1 sm:grid-cols-2 gap-4">
                {featured.map((dev, idx) => (
                  <div key={idx} onClick={() => selectDevice(dev.name)} className="hover:scale-[1.01] transition-transform">
                    <div className="bg-white border rounded-lg shadow p-4">
                      <h3 className="text-lg font-bold mb-2">{dev.name}</h3>
                      <p className="text-sm text-gray-600">Model: <span className="font-medium">{dev.device.deviceName}</span></p>
                      <p className="text-sm text-gray-600">Vendor: <span className="font-medium">{dev.device.vendorName}</span></p>
                    </div>
                  </div>
                ))}
              </div>
            </div>
          )}
        </div>
      </main>
    </div>
  );
}